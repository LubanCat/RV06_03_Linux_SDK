#!/bin/bash
#
# Copyright (c) 2026 Rockchip Electronics Co., Ltd
#
# SPDX-License-Identifier: GPL-2.0
#

set -euo pipefail

usage() {
	echo "Usage: "
	echo "    $0 -f <content_file>"
	echo
	echo "Example: "
	echo "    ./scripts/release-doc.sh -f scripts/doc-template.txt"
	exit 1
}

plat=
build_commit=
summary_severity=
content_file=
declare -a new_items_cn=()
declare -a new_items_en=()
declare -a fix_items_cn=()
declare -a fix_items_en=()

while [ "$#" -gt 0 ]; do
	case "$1" in
	-f)
		shift
		[ "$#" -gt 0 ] || usage
		content_file="$1"
		;;
	*)
		usage
		;;
	esac
	shift
done

[ -n "$content_file" ] || usage

script_dir=$(cd "$(dirname "$0")" && pwd)
repo_dir=$(cd "$script_dir/.." && pwd)
plat_upper=$(printf '%s' "$plat" | tr '[:lower:]' '[:upper:]')
current_date=$(date +%F)

trim_text() {
	local text="$1"

	text="${text#"${text%%[![:space:]]*}"}"
	text="${text%"${text##*[![:space:]]}"}"
	printf '%s' "$text"
}

normalize_separators() {
	local text="$1"

	text=$(printf '%s' "$text" | sed -E 's/[[:space:]]*:[[:space:]]*/: /; s/[[:space:]]*;[[:space:]]*/;/g')
	printf '%s' "$text"
}

append_new_item() {
	local lang="$1"
	local item="$2"

	if [ "$lang" = "CN" ]; then
		new_items_cn+=("$item")
	else
		new_items_en+=("$item")
	fi
}

append_fix_item() {
	local lang="$1"
	local item="$2"

	if [ "$lang" = "CN" ]; then
		fix_items_cn+=("$item")
	else
		fix_items_en+=("$item")
	fi
}

detect_lang() {
	local text="$1"

	if printf '%s' "$text" | perl -CS -ne 'exit(/\p{Han}/ ? 0 : 1)'; then
		printf 'CN'
	else
		printf 'EN'
	fi
}

load_content_items() {
	local line
	local trimmed_line
	local normalized_line
	local total
	local lang
	local entry_type
	local entry_body
	local meta_key
	local col1
	local col2
	local col3
	local col4
	local extra
	local item

	[ -n "$content_file" ] || return 0

	[ -f "$content_file" ] || {
		echo "Missing content file: $content_file" >&2
		exit 1
	}

	while IFS= read -r line || [ -n "$line" ]; do
		trimmed_line=$(trim_text "$line")
		[ -n "$trimmed_line" ] || continue
		[[ "$trimmed_line" == \#* ]] && continue
		normalized_line=$(normalize_separators "$trimmed_line")

		if [[ "$normalized_line" =~ ^([[:alnum:]_]+):[[:space:]]*(.*)$ ]]; then
			entry_type=$(printf '%s' "${BASH_REMATCH[1]}" | tr '[:upper:]' '[:lower:]')
			entry_body=$(trim_text "${BASH_REMATCH[2]}")
		else
			echo "Invalid content line: $line" >&2
			exit 1
		fi

		[ -n "$entry_body" ] || {
			echo "Invalid content line: $line" >&2
			exit 1
		}

		case "$entry_type" in
		plat|file|build_commit|severity)
			meta_key="$entry_type"
			case "$meta_key" in
			plat)
				plat="$entry_body"
				plat_upper=$(printf '%s' "$plat" | tr '[:lower:]' '[:upper:]')
				;;
			file)
				release_file="$entry_body"
				;;
			build_commit)
				build_commit="$entry_body"
				;;
			severity)
				summary_severity="$entry_body"
				;;
			esac
			;;
		new)
			lang=$(detect_lang "$entry_body")
			append_new_item "$lang" "$entry_body"
			;;
		fixed)
			IFS=';；' read -r col1 col2 col3 col4 extra <<EOF
$entry_body
EOF
			col1=$(trim_text "$col1")
			col2=$(trim_text "$col2")
			col3=$(trim_text "$col3")
			col4=$(trim_text "$col4")
			extra=$(trim_text "$extra")

			if [ -n "$extra" ] || [ -z "$col1" ] || [ -z "$col2" ] || [ -z "$col3" ] || [ -z "$col4" ]; then
				echo "Invalid fixed content line: $line" >&2
				exit 1
			fi

			lang=$(detect_lang "$entry_body")
			format_severity "$lang" "$col1" >/dev/null
			item="${col1}|${col2}|${col3}|${col4}"
			append_fix_item "$lang" "$item"
			;;
		*)
			echo "Unknown content type: $line" >&2
			exit 1
			;;
		esac
	done < "$content_file"

	[ -n "$plat" ] || {
		echo "Missing plat in content file: $content_file" >&2
		exit 1
	}
	[ -n "$release_file" ] || {
		echo "Missing file in content file: $content_file" >&2
		exit 1
	}
	[ -n "$build_commit" ] || {
		echo "Missing commit in content file: $content_file" >&2
		exit 1
	}
	if [ "${#new_items_cn[@]}" -eq 0 ] && [ "${#new_items_en[@]}" -eq 0 ] && \
	   [ "${#fix_items_cn[@]}" -eq 0 ] && [ "${#fix_items_en[@]}" -eq 0 ]; then
		echo "ERROR: Can't find \"New:\" or \"Fixed:\" in $content_file" >&2
		exit 1
	fi
	total=$((${#fix_items_cn[@]} + ${#fix_items_en[@]}))
	: "$total"
}

get_section_title() {
	if [ -n "$release_file" ]; then
		printf '%s\n' "$release_file"
	else
		printf 'rkxxx\n'
	fi
}

get_table_file() {
	printf '%s\n' "$release_file"
}

get_summary_header() {
	local lang="$1"

	if [ "$lang" = "CN" ]; then
		printf '%s\n' '| 时间 | 文件 | 编译 commit | 重要程度 |'
		printf '%s\n' '| ---- | :--- | ----------- | -------- |'
	else
		printf '%s\n' '| Date | File | Build commit | Severity |'
		printf '%s\n' '| ---- | :--- | ------------ | -------- |'
	fi
}

get_fix_header() {
	local lang="$1"

	if [ "$lang" = "CN" ]; then
		printf '%s\n' '| Index | 重要程度 | 更新说明 | 问题现象 | 问题来源 |'
		printf '%s\n' '| ----- | -------- | -------- | -------- | -------- |'
	else
		printf '%s\n' '| Index | Severity | Update | Issue description | Issue source |'
		printf '%s\n' '| ----- | -------- | ------ | ----------------- | ------------ |'
	fi
}

print_summary_section() {
	local lang="$1"
	local section_title="$2"
	local table_file="$3"
	local summary_commit="$4"
	local summary_severity="$5"

	printf '## %s\n\n' "$section_title"
	get_summary_header "$lang"
	printf '| %s | %s | %s | %s |\n\n' \
		"$current_date" "$table_file" "$summary_commit" "$summary_severity"
}

format_severity() {
	local lang="$1"
	local severity="$2"

	case "$severity" in
	"")
		printf '\n'
		;;
	紧急|critical)
		if [ "$lang" = "CN" ]; then
			printf '紧急\n'
		else
			printf 'critical\n'
		fi
		;;
	重要|important)
		if [ "$lang" = "CN" ]; then
			printf '重要\n'
		else
			printf 'important\n'
		fi
		;;
	普通|moderate)
		if [ "$lang" = "CN" ]; then
			printf '普通\n'
		else
			printf 'moderate\n'
		fi
		;;
	*)
		echo "Invalid severity: $severity" >&2
		usage
		;;
	esac
}

load_content_items

print_new_items() {
	local lang="$1"
	local -a items=()
	local i
	local content

	if [ "$lang" = "CN" ] && [ "${#new_items_cn[@]}" -gt 0 ]; then
		items=("${new_items_cn[@]}")
	elif [ "$lang" = "EN" ] && [ "${#new_items_en[@]}" -gt 0 ]; then
		items=("${new_items_en[@]}")
	fi

	if [ "${#items[@]}" -gt 0 ]; then
		i=1
		for content in "${items[@]}"; do
			if [ "$lang" = "CN" ]; then
				if [[ ! "$content" =~ [。！？]$ ]]; then
					content="${content}。"
				fi
			else
				if [[ ! "$content" =~ [.!?]$ ]]; then
					content="${content}."
				fi
			fi
			printf '%d. %s\n' "$i" "$content"
			i=$((i + 1))
		done
		return 0
	fi
}

print_new_section() {
	local lang="$1"

	if [ "$lang" = "CN" ] && [ "${#new_items_cn[@]}" -eq 0 ]; then
		return 0
	fi
	if [ "$lang" = "EN" ] && [ "${#new_items_en[@]}" -eq 0 ]; then
		return 0
	fi

	cat <<'EOF'
### New

EOF
	print_new_items "$lang"
	echo
}

print_fix_section() {
	local lang="$1"
	local i
	local -a items=()
	local item
	local item_severity
	local col1
	local col2
	local col3
	local col4

	if [ "$lang" = "CN" ]; then
		items=("${fix_items_cn[@]}")
	else
		items=("${fix_items_en[@]}")
	fi

	[ "${#items[@]}" -gt 0 ] || return 0

	printf '### Fixed\n\n'
	get_fix_header "$lang"

	i=1
	while [ "$i" -le "${#items[@]}" ]; do
		item="${items[$((i - 1))]}"
		IFS='|' read -r item_severity col1 col2 col3 col4 <<EOF
$item
EOF
		[ -z "$col4" ] || {
			echo "Invalid fix item: $item" >&2
			exit 1
		}
		item_severity=$(format_severity "$lang" "$item_severity")
		printf '| %d     | %s | %s | %s | %s |\n' \
			"$i" "$item_severity" "$col1" "$col2" "$col3"
		i=$((i + 1))
	done
	echo
}

insert_template() {
	local target="$1"
	local lang="$2"
	local tmp
	local section_title
	local table_file
	local header_severity=

	section_title=$(get_section_title)
	table_file=$(get_table_file)
	if [ -n "$summary_severity" ]; then
		header_severity="$summary_severity"
	elif [ "$lang" = "CN" ] && [ "${#fix_items_cn[@]}" -gt 0 ]; then
		IFS='|' read -r header_severity _ <<EOF
${fix_items_cn[0]}
EOF
	elif [ "$lang" = "EN" ] && [ "${#fix_items_en[@]}" -gt 0 ]; then
		IFS='|' read -r header_severity _ <<EOF
${fix_items_en[0]}
EOF
	else
		header_severity="moderate"
	fi
	header_severity=$(format_severity "$lang" "$header_severity")

	if [ ! -f "$target" ]; then
		echo "Missing file: $target" >&2
		exit 1
	fi

	tmp=$(mktemp)
	{
		sed -n '1p' "$target"
		echo
		print_summary_section "$lang" "$section_title" "$table_file" "$build_commit" "$header_severity"
		print_new_section "$lang"
		print_fix_section "$lang"
		echo "------"
		sed -n '2,$p' "$target"
	} > "$tmp"
	mv "$tmp" "$target"
}

insert_template \
	"$repo_dir/doc/release/${plat_upper}_CN.md" \
	"CN"
insert_template \
	"$repo_dir/doc/release/${plat_upper}_EN.md" \
	"EN"
git diff HEAD doc/release/
