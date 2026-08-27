#!/bin/bash
set -e

ARG_COMMIT=$1
DIFF_SUBSET="scripts/.diff_*"
DIFF_DOC_ALL="scripts/.diff_all.txt"
DIFF_DOC_FIXED="scripts/.diff_fixed.txt"
LAST_SEVERITY=
LAST_DOC=

# Helper function to check if severity is valid
function is_valid_severity()
{
	local severity="$1"
	local critical="$2"
	local important="$3"
	local moderate="$4"
	[ "${severity}" = "${critical}" ] || [ "${severity}" = "${important}" ] || [ "${severity}" = "${moderate}" ]
}

function check_doc()
{
	local TOP_SEVERITY LANGUAGE=$1

	if [ "${LANGUAGE}" == "EN" ] ; then
		SVT_CRITICAL="critical"
		SVT_IMPORTANT="important"
		SVT_MODERATE="moderate"
		DOC=`git log ${ARG_COMMIT} -1 --name-only | sed -n "/_EN\.md/p"`
	else
		SVT_CRITICAL="紧急"
		SVT_IMPORTANT="重要"
		SVT_MODERATE="普通"
		DOC=`git log ${ARG_COMMIT} -1 --name-only | sed -n "/_CN\.md/p"`
	fi

	echo "    - ${DOC}"

	# check DOS encoding
	git show ${ARG_COMMIT} -1 ${DOC} | sed -n "/^+/p" > ${DIFF_DOC_ALL}
	cp ${DIFF_DOC_ALL} ${DIFF_DOC_ALL}.dos
	if ! dos2unix ${DIFF_DOC_ALL}.dos >/dev/null 2>&1; then
		echo "ERROR: dos2unix failed. Install it by: sudo apt-get install dos2unix"
		exit 1
	fi
	CSUM1=`md5sum ${DIFF_DOC_ALL} | awk '{ print $1 }'`
	CSUM2=`md5sum ${DIFF_DOC_ALL}.dos | awk '{ print $1 }'`
	if [ "${CSUM1}" != "${CSUM2}" ]; then
		echo "ERROR: ${DOC} is DOS encoding. Fix it by: 'dos2unix ${DOC}'"
		exit 1
	fi

	TITLE=`sed -n "/^+## /p" ${DIFF_DOC_ALL} | tr -d " +#"`

	# Parse table line once (avoid repeated sed/awk calls)
	TABLE_LINE=$(sed -n "/^+| 20[0-9][0-9]-/p" ${DIFF_DOC_ALL} | tr -d " ")
	if [ -z "${TABLE_LINE}" ]; then
		echo "ERROR: ${DOC}: No valid date table line found (expected format: | YYYY-MM-DD | ...)"
		exit 1
	fi
	DATE=$(echo "$TABLE_LINE" | awk -F "|" '{ print $2 }')
	YEAR=$(echo "$DATE" | awk -F "-" '{ print $1 }')
	MON=$(echo "$DATE" | awk -F "-" '{ print $2 }')
	FILE=$(echo "$TABLE_LINE" | awk -F "|" '{ print $3 }')
	COMMIT=$(echo "$TABLE_LINE" | awk -F "|" '{ print $4 }')
	SEVERITY=$(echo "$TABLE_LINE" | awk -F "|" '{ print $5 }')

	# Get last 3 lines at once (avoid repeated tail calls)
	END_LINES=($(tail -n 3 ${DIFF_DOC_ALL}))
	if [ ${#END_LINES[@]} -lt 3 ]; then
		echo "ERROR: ${DOC}: Insufficient lines in document (need at least 3 lines at end)"
		exit 1
	fi
	END_LINE_3="${END_LINES[0]}"
	END_LINE_2="${END_LINES[1]}"
	END_LINE_1="${END_LINES[2]}"

	HOST_YEAR=`date +%Y`
	HOST_MON=`date +%m`
	# echo "### ${COMMIT}, ${SEVERITY}, ${TITLE}, ${FILE}"

	# check blank line after Heading 1
	HEADING_1=`sed -n '1p' ${DOC}`
	if sed -n '2p' ${DOC} | grep -q [a-z,A-Z] ; then
		echo "ERROR: ${DOC}: Please add blank line after '${HEADING_1}'"
		exit 1
	fi

	# check space
	if sed -n "/##/p" ${DOC} | grep -v '## [a-z,A-Z]' ; then
		echo "ERROR: ${DOC}: Please only 1 space between '#' and word"
		exit 1
	fi

	# check new content location
	if ! git show ${ARG_COMMIT} -1 ${DOC} | grep -q 'Release Note' ; then
		echo "ERROR: ${DOC}: Please add new content at the top but not bottom"
		exit 1
	fi

	# check added release note section count
	SECTION_SUM=`sed -n '/^+## /p' ${DIFF_DOC_ALL} | wc -l`
	if [ "${SECTION_SUM}" -gt 1 ]; then
		echo "ERROR: ${DOC}: There are more than one file changed section:"
		sed -n '/^+## /p' ${DIFF_DOC_ALL} | sed 's/^+//'
		exit 1
	fi

	# check title
	if grep -Eq '### WARN|### WARNING|### Warning|### warn|### warning' ${DIFF_DOC_ALL} ; then
		echo "ERROR: ${DOC}: Please use '### Warn'"
		exit 1
	fi

	if grep -Eq '### NEW|### new' ${DIFF_DOC_ALL} ; then
		echo "ERROR: ${DOC}: Please use '### New'"
		exit 1
	fi

	if grep -Eq '### FIXED|### fixed' ${DIFF_DOC_ALL} ; then
		echo "ERROR: ${DOC}: Please use '### Fixed'"
		exit 1
	fi

	# check added body format in new "### New" section
	NEW_BODY_NO_INDEX=`awk '
		BEGIN { in_new=0 }
		/^\+### New$/ {
			in_new=1
			next
		}
		in_new && /^\+## / {
			in_new=0
		}
		in_new && /^\+### / {
			in_new=0
		}
		in_new && /^\+------$/ {
			in_new=0
		}
		in_new && /^\+[[:space:]]*$/ {
			next
		}
		in_new && /^\+/ {
			if ($0 !~ /^\+[0-9]+\.[[:blank:]]/) {
				print substr($0, 2)
			}
		}
	' ${DIFF_DOC_ALL}`
	if [ -n "${NEW_BODY_NO_INDEX}" ]; then
		echo "ERROR: ${DOC}: Please add index prefix like '1. Add ...' for each added line in '### New':"
		echo "${NEW_BODY_NO_INDEX}"
		exit 1
	fi

	# check year/month
	if [ "${ARG_COMMIT}" != "" -a "${ARG_COMMIT}" != "" ]; then
		if [ "${HOST_YEAR}" != "${YEAR}" ]; then
			echo "ERROR: ${DOC}: '${DATE}' is wrong, the year should be ${HOST_YEAR}"
			exit 1
		fi

		if [ "${HOST_MON}" != "${MON}" ]; then
			echo "ERROR: ${DOC}: '${DATE}' is wrong, the month should be ${HOST_MON}"
			exit 1
		fi
	fi

	# check TAB before index of 'New' body
	if grep -q $'\t[0-9]' ${DOC} ; then
		echo "ERROR: ${DOC}: Don't add TAB before index:"
		grep $'\t[0-9]' ${DOC}
		exit 1
	fi

	# check upper case and line end
	if [ "${LANGUAGE}" == "EN" ] ; then
		if grep -q '^[0-9]\. [a-z]' ${DOC} ; then
			echo "ERROR: ${DOC}: Please use upper case of first word(i.e. \"1. add ..\" => \"1. Add ...\"):"
			grep '^[0-9]\. [a-z]' ${DOC}
			exit 1
		fi

		# check end with '.'
		if sed -n '/^[0-9]\. [A-Z]/p' ${DOC} | grep -q '[^.]$' ; then
			echo "ERROR: ${DOC}: Please end line with '.'"
			grep '^[0-9]\. [A-Z]' ${DOC} | grep '[^.]$'
			exit 1
		fi

		# check Chinese language
		if grep -P '[\x{4e00}-\x{9fa5}]' ${DOC} ; then
			echo "ERROR: ${DOC}: The Chinese language was found"
			exit 1
		fi
	else
		# check end with '。'
		if sed -n '/^[0-9]\. /p' ${DOC} | grep -q '[^。]$' ; then
			echo "ERROR: ${DOC}: Please end line with '。'"
			grep '^[0-9]\. ' ${DOC} | grep '[^。]$'
			exit 1
		fi
	fi

	# check space after index of 'New' body
	SUM1=`grep '^[0-9]\.' ${DOC} | wc -l`
	SUM2=`grep '^[0-9]\.[[:blank:]]' ${DOC} | wc -l`
	if [ "$SUM1" != "$SUM2" ]; then
		echo "ERROR: ${DOC}: Please add space after index (e.g: '1. ' but not '1.'):"
		grep '^+[0-9]\.' ${DIFF_DOC_ALL}
		exit 1
	fi

	# check standalone file
	if ! echo ${FILE} | grep -Eq '\.bin|\.elf|\.img' ; then
		echo "ERROR: ${DOC}: '${FILE}' missing the file format suffix"
		exit 1
	fi
	if ! echo ${FILE} | grep -q { ; then
		if ! git log ${ARG_COMMIT} -1 --name-only | grep -q ${FILE}; then
			echo "ERROR: ${DOC}: '${FILE}' is not updated in this patch"
			exit 1
		fi
	fi

	# check title
	if [ "${TITLE}" != "${FILE}" ]; then
		echo "ERROR: ${DOC}: Title '${TITLE}' is not match with '${FILE}'"
		exit 1
	fi

	# check commit
	COMMIT=${COMMIT//#/ }
	for LIST in ${COMMIT}; do
		CMT=`echo ${LIST} | cut -d : -f 2`
		if ! git log ${ARG_COMMIT} -1 | grep -q ${CMT} ; then
			echo "ERROR: ${DOC}: '${CMT}' is not match in commit message"
			exit 1
		fi

		if ! echo ${FILE} | grep -q { ; then
			if echo ${FILE} | grep -Eq 'spl_|tpl_|bl31_|bl32_|tee_' ; then
				FILE_PATH=`find -name ${FILE}`
				if [ -z "${FILE_PATH}" ]; then
					echo "ERROR: ${DOC}: No ${FILE}"
					exit 1
				fi
				if ! strings ${FILE_PATH} | grep -q ${CMT} ; then
					echo "ERROR: ${DOC}: ${FILE} is not build from '${CMT}'"
					exit 1
				fi
			fi
		fi
	done

	# check severity
	if ! is_valid_severity "${SEVERITY}" "${SVT_CRITICAL}" "${SVT_IMPORTANT}" "${SVT_MODERATE}"; then
		echo "ERROR: ${DOC}: Unknown main severity: ${SEVERITY}"
		exit 1
	fi

	# check horizontal line
	if [ "${END_LINE_1}" != "+" ]; then
		echo "ERROR: ${DOC} ${END_LINE_1}: Please add blank line after horizontal line '------'"
		exit 1
	fi
	if [ "${END_LINE_2}" != "+------" ]; then
		echo "ERROR: ${DOC}: Please add horizontal line '------' at the last of new content"
		exit 1
	fi
	if [ "${END_LINE_3}" != "+" ]; then
		echo "ERROR: ${DOC}: Please add blank line before horizontal line '------'"
		exit 1
	fi

	# check 'Fixed' content
	if grep -q "^+### Fixed" ${DIFF_DOC_ALL} ; then
		awk -v RS='### Fixed' 'END{printf "%s", $0}' ${DIFF_DOC_ALL} > ${DIFF_DOC_FIXED}
		sed -i "/^$/d"    ${DIFF_DOC_FIXED}
		sed -i "/Index/d" ${DIFF_DOC_FIXED}
		sed -i "/---/d"   ${DIFF_DOC_FIXED}
		sed -i "/^+$/d"   ${DIFF_DOC_FIXED}

		while read LINE
		do
			EACH_SEVERITY=`echo "${LINE}" | awk -F "|" '{ print $3 }' | tr -d " "`
			if ! is_valid_severity "${EACH_SEVERITY}" "${SVT_CRITICAL}" "${SVT_IMPORTANT}" "${SVT_MODERATE}"; then
				if [ -z "${EACH_SEVERITY}" ]; then
					echo "ERROR: ${DOC}: No severity found, please use Table to list what you '### Fixed'"
				else
					echo "ERROR: ${DOC}: Unknown severity: ${EACH_SEVERITY}"
				fi
				exit 1
			fi

			# echo "## EACH: $EACH_SEVERITY"
			if [ -z "${TOP_SEVERITY}" ]; then
				TOP_SEVERITY="${EACH_SEVERITY}"
			elif [ "${TOP_SEVERITY}" == "${SVT_MODERATE}" ]; then
				if [ "${EACH_SEVERITY}" == "${SVT_CRITICAL}" -o "${EACH_SEVERITY}" == "${SVT_IMPORTANT}" ]; then
					TOP_SEVERITY="${EACH_SEVERITY}"
				fi
			elif [ "${TOP_SEVERITY}" == "${SVT_IMPORTANT}" ]; then
				if [ "${EACH_SEVERITY}" == "${SVT_CRITICAL}" ]; then
					TOP_SEVERITY="${EACH_SEVERITY}"
				fi
			fi
		done < ${DIFF_DOC_FIXED}

		if [ "${SEVERITY}" != "${TOP_SEVERITY}" ]; then
			echo "ERROR: ${DOC}: Top severity should be '${TOP_SEVERITY}' as it's the highest level of all sub severity"
			exit 1
		fi

		# check top severity miss match
		if [ ! -z ${LAST_SEVERITY} ]; then
			if [ "${LAST_SEVERITY}" == "普通" -a "${TOP_SEVERITY}" != "moderate" ]; then
				MISS_MATCH="y"
			elif [ "${LAST_SEVERITY}" == "重要" -a "${TOP_SEVERITY}" != "important" ]; then
				MISS_MATCH="y"
			elif [ "${LAST_SEVERITY}" == "紧急" -a "${TOP_SEVERITY}" != "critical" ]; then
				MISS_MATCH="y"
			elif [ "${LAST_SEVERITY}" == "moderate" -a "${TOP_SEVERITY}" != "普通" ]; then
				MISS_MATCH="y"
			elif [ "${LAST_SEVERITY}" == "important" -a "${TOP_SEVERITY}" != "重要" ]; then
				MISS_MATCH="y"
			elif [ "${LAST_SEVERITY}" == "critical" -a "${TOP_SEVERITY}" != "紧急" ]; then
				MISS_MATCH="y"
			fi

			if [ "${MISS_MATCH}" == "y" ]; then
				echo "ERROR: ${DOC}: top Severity is '${SEVERITY}', while ${LAST_DOC}: top Severity is '${LAST_SEVERITY}'"
				echo "       Available Severity types are: moderate(普通), important(重要), critical(紧急)"
				exit 1
			fi
		fi

		LAST_SEVERITY="${SEVERITY}"
		LAST_DOC="${DOC}"
	fi
}

function check_docs()
{
	if git log ${ARG_COMMIT} -1 --name-only | sed -n '5p' | grep -Eq '^    Revert "' ; then
		return;
	fi

	echo "Checking doc ..."
	if git log ${ARG_COMMIT} -1 --name-only --format='' | grep -Eq '\.bin|\.elf' ; then
		DOC_CN=`git log ${ARG_COMMIT} -1 --name-only | sed -n "/_CN\.md/p"`
		DOC_EN=`git log ${ARG_COMMIT} -1 --name-only | sed -n "/_EN\.md/p"`
		if [ -z "${DOC_CN}" -o -z "${DOC_EN}" ]; then
			echo "ERROR: Should update CN and EN Release-Note when .bin/elf changed"
			exit 1
		fi

		NUM=`git log ${ARG_COMMIT} -1 --name-only | sed -n "/\.md/p" | wc -l`
		if [ ${NUM} -gt 2 ]; then
			echo "ERROR: More than 2 release note are updated"
			exit 1
		fi

		if ! which dos2unix > /dev/null 2>&1 ; then
			echo "ERROR: No 'dos2unix'. Fix by: sudo apt-get install dos2unix"
			exit 1
		fi

		check_doc CN
		check_doc EN
	fi

	rm -f ${DIFF_SUBSET}
}

function pack_loader_image()
{
	for FILE in `ls ./RKBOOT/*MINIALL*.ini`
	do
		if [ "${FILE}" = "./RKBOOT/RK302AMINIALL.ini" -o \
			 "${FILE}" = "./RKBOOT/RK30BMINIALL.ini" -o \
			 "${FILE}" = "./RKBOOT/RK30MINIALL.ini" -o \
			 "${FILE}" = "./RKBOOT/RK310BMINIALL.ini" ]; then
			continue;
		fi

		if grep -q '^PATH=img/' ${FILE}; then
			continue;
		fi

		echo "Pack loader: ${FILE}"
		./tools/boot_merger ${FILE}
		rm -f *loader*.bin *download*.bin *idblock*.img
		echo
	done
}

function check_trust_ini_extra_files()
{
	local FILE=$1
	local ITEM
	local EXTRA_FILE

	for ITEM in `sed -n '/^\(MCU\|MCU[0-9]\+\|LOAD[0-9]\+\|INIT[0-9]\+\)=/p' ${FILE} | tr -d '\r'`
	do
		EXTRA_FILE=$(echo ${ITEM} | cut -d= -f2 | cut -d, -f1)
		# Ignore files:
		if [ "${EXTRA_FILE}" == "bin/rv11/rtthread.bin" ]; then
			continue
		fi
		# Verify
		if [ ! -e "${EXTRA_FILE}" ]; then
			echo "ERROR: ${FILE}: missing file ${EXTRA_FILE}"
			exit 1
		fi
	done
}

function extract_ini_bin_refs()
{
	local FILE=$1

	sed -n 's/^[^=]\+=\(bin\/[^,[:space:]\r]*\).*$/\1/p' ${FILE} \
		| tr -d '\r' \
		| sort -u
}

function check_ini_unused_bin_files()
{
	local FILE
	local OLD_REF
	local TMP_OLD_INI="scripts/.checkpatch_old_ini.tmp"
	local TMP_OLD_REFS="scripts/.checkpatch_old_refs.tmp"
	local TARGET_COMMIT
	local PARENT_COMMIT

	TARGET_COMMIT=${ARG_COMMIT:-HEAD}
	PARENT_COMMIT=$(git rev-parse ${TARGET_COMMIT}^ 2>/dev/null) || return

	if ! git log ${TARGET_COMMIT} -1 --name-only --format='' | grep -Eq '\.ini$'; then
		return
	fi

	echo "Checking unused file refs ..."
	for FILE in $(git log ${TARGET_COMMIT} -1 --name-only --format='' | sed -n '/\.ini$/p'); do
		if ! git diff-tree --no-commit-id --name-status -r ${TARGET_COMMIT} | grep -Eq "^[MD][[:space:]]+${FILE}$"; then
			continue
		fi

		if ! git show ${PARENT_COMMIT}:${FILE} > ${TMP_OLD_INI} 2>/dev/null; then
			continue
		fi

		extract_ini_bin_refs ${TMP_OLD_INI} > ${TMP_OLD_REFS}
		while read OLD_REF; do
			if [ -z "${OLD_REF}" ]; then
				continue
			fi

			if grep -R -q -F "${OLD_REF}" --include='*.ini' RKBOOT RKTRUST; then
				continue
			fi

			if [ -e "${OLD_REF}" ]; then
				echo "ERROR: Please delete ${OLD_REF}, which is no longer referenced by any ini"
				rm -f ${TMP_OLD_INI} ${TMP_OLD_REFS}
				exit 1
			fi
		done < ${TMP_OLD_REFS}
	done

	rm -f ${TMP_OLD_INI} ${TMP_OLD_REFS}
}

function pack_trust_image()
{
	# Pack 32-bit trust
	for FILE in `ls ./RKTRUST/*TOS*.ini`
	do
		if ! test -s ${FILE}; then
			continue;
		elif ! grep -q 'TOS' ${FILE}; then
			continue;
		elif grep -q '^PATH=img/' ${FILE}; then
			continue;
		fi

		echo "Pack trust: ${FILE}"
		check_trust_ini_extra_files ${FILE}
		# Parse orignal path
		TOS=`sed -n "/TOS=/s/TOS=//p" ${FILE}|tr -d '\r'`
		TOS_TA=`sed -n "/TOSTA=/s/TOSTA=//p" ${FILE}|tr -d '\r'`

		# replace "./tools/rk_tools/" with "./" to compatible legacy ini content of rkdevelop branch
		TOS=$(echo ${TOS} | sed "s/tools\/rk_tools\//\.\//g")
		TOS_TA=$(echo ${TOS_TA} | sed "s/tools\/rk_tools\//\.\//g")

		if [ x${TOS_TA} != x -a x${TOS} != x ]; then
			./tools/loaderimage --pack --trustos ${TOS} ./trust.img 0x68400000
			./tools/loaderimage --pack --trustos ${TOS_TA} ./trust_with_ta.img 0x68400000
		elif [ ${TOS} ]; then
			./tools/loaderimage --pack --trustos ${TOS} ./trust.img 0x68400000
		elif [ ${TOS_TA} ]; then
			./tools/loaderimage --pack --trustos ${TOS_TA} ./trust.img 0x68400000
		else
			exit 1
		fi
		rm -f trust*.img
		echo
	done

	# Pack 64-bit trust
	for FILE in `ls ./RKTRUST/*TRUST*.ini`
	do
		if grep -q '^PATH=img/' ${FILE}; then
			continue;
		fi

		echo "Pack trust: ${FILE}"
		check_trust_ini_extra_files ${FILE}
		./tools/trust_merger ${FILE}
		rm -f trust*.img
		echo
	done
}

function check_dirty()
{
	echo "Checking clean ..."
	for FILE in `find -name '*spl*.bin' -o -name '*tpl*.bin' -o -name '*usbplug*.bin' -o -name '*bl31*.elf' -o -name '*bl32*.bin'`; do
		if [[ "${FILE}" == *fspi1* ]]; then
			echo "Skip clean: ${FILE}"
			continue;
		fi
		echo "    - ${FILE}"
		if strings ${FILE} | grep '\-dirty ' ; then
			echo "ERROR: ${FILE} is dirty"
			exit 1
		fi
	done
}

function check_stripped()
{
	echo "Checking strip ..."
	for FILE in `find -name '*bl31*.elf'`; do
		echo "    - ${FILE}"
		INFO=`file ${FILE}`
		if echo ${INFO} | grep -q "not stripped" ; then
			echo "ERROR: ${FILE} is not stripped"
			exit 1
		fi
	done
}

function check_mode()
{
	echo "Checking file mode..."
	if git whatchanged ${ARG_COMMIT} -1 --oneline | sed -n '/RKBOOT\//p; /RKTRUST\//p; /bin\//p; /doc\//p;' | awk '{ print $2 }' | grep -q 755 ; then
		git whatchanged ${ARG_COMMIT} -1 --oneline | sed -n '/RKBOOT\//p; /RKTRUST\//p; /bin\//p; /doc\//p;' | grep 755
		echo "ERROR: Set 644 file permission but not 755."
		exit 1
	fi
}

check_commit_message()
{
	if git log ${ARG_COMMIT} -1 --name-only | sed -n '5p' | grep -Eq '^    Revert "' ; then
		return;
	fi

	if ! git log ${ARG_COMMIT} -1 --name-only --format='' | grep -Eq '\.bin|\.elf' ; then
		if ! git log ${ARG_COMMIT} -1 --name-only --format='' | grep -Eq 'syscfg' ; then
			return
		fi
	fi

	echo "Checking commit message format..."
	MSG=$(git log ${ARG_COMMIT} -1 --pretty=format:"%B")

	# 1. Header capitalization
	if echo "$MSG" | grep -q '^build from'; then
		echo "ERROR: Please change 'build from' to 'Build from'"
		exit 1
	fi
	if echo "$MSG" | grep -q '^update feature'; then
		echo "ERROR: Please change 'update feature' to 'Update feature'"
		exit 1
	fi

	# 2. 'Update feature' must exist and be unique
	if ! echo "$MSG" | grep -q '^Update feature'; then
		echo "ERROR: Missing 'Update feature' section"
		exit 1
	fi

	# 2.1 Check blank line before 'Update feature'
	UPDATE_FEATURE_LINE=$(echo "$MSG" | grep -n '^Update feature' | cut -d: -f1)
	if [ -n "$UPDATE_FEATURE_LINE" ]; then
		PREV_LINE=$((UPDATE_FEATURE_LINE - 1))
		PREV_LINE_CONTENT=$(echo "$MSG" | sed -n "${PREV_LINE}p")
		if [ -n "$PREV_LINE_CONTENT" ]; then
			echo "ERROR: Please add blank line before 'Update feature'"
			exit 1
		fi
	fi

	# 3. Line 3 must be 'Build from'
	if ! sed -n '3p' <<< "$MSG" | grep -q '^Build from'; then
		echo "ERROR: Don't add content before 'Build from'"
		exit 1
	fi

	# 3.1 Check blank line before 'Build from' (line 2)
	LINE_2=$(sed -n '2p' <<< "$MSG")
	if [ -n "$LINE_2" ]; then
		echo "ERROR: Please add blank line before 'Build from'"
		exit 1
	fi

	# 4. Split by blank lines into sections. The first line of a section must
	# start at column 1; following lines must use a single TAB indent.
	SECTION_LINE_NO=0
	while IFS= read -r line; do
		if [ -z "$line" ]; then
			SECTION_LINE_NO=0
			continue
		fi

		case "$line" in
		Change-Id:*|Signed-off-by:*)
			continue
			;;
		esac

		SECTION_LINE_NO=$((SECTION_LINE_NO + 1))
		if [ "$SECTION_LINE_NO" -eq 1 ]; then
			if [[ ! "$line" =~ ^[^[:space:]] ]]; then
				echo "ERROR: Please start from line begin:"
				printf '%s\n' "$line"
				exit 1
			fi
		else
			if [[ ! "$line" =~ ^$'\t'[^[:space:]] ]]; then
				echo "ERROR: Please start with single TAB indent:"
				printf '%s\n' "$line"
				exit 1
			fi
		fi
	done <<< "$MSG"
}

function check_version()
{
	echo "Checking fwver..."
	git whatchanged ${ARG_COMMIT} -1 --name-only | sed -n '/bin\//p' | sed -n '/ddr/p; /tpl/p; /spl/p; /bl31/p; /bl32/p; /tee/p;' | while read FILE; do
		if ! test -f ${FILE}; then
			continue
		fi

		NAME_VER=`echo ${FILE} | grep -o 'v[0-9][.][0-9][0-9]'`
		# ignore version < v1.00
		if [[ "${NAME_VER}" == *v0.* ]]; then
			continue
		fi

		# ignore rk3308
		if [[ "${FILE}" == *rk3308_bl31_* ]]; then
			continue
		fi

		# ignore mos spl
		if [[ "${FILE}" == *_spl*primary_* ]]; then
			continue
		fi
		if [[ "${FILE}" == *_spl*secondary_* ]]; then
			continue
		fi

		if ! strings ${FILE} | grep -q 'fwver: ' ; then
			echo "ERROR: ${FILE}: No \"fwver: \" string found in binary"
			exit 1
		fi

		echo "    - ${FILE}: '${NAME_VER}'"
		FW_VER=`strings ${FILE} | grep -m 1 -o 'fwver: v[1-9][.][0-9][0-9]' | awk '{ print $2 }'`
		if [ "${NAME_VER}" != "${FW_VER}" ] ; then
			echo "ERROR: ${FILE}: file version is '${NAME_VER}', but fw version is '${FW_VER}'."
			exit 1
		fi
	done
}

function finish()
{
	echo "OK, everything is nice."
	echo
}

check_commit_message
check_mode
check_ini_unused_bin_files
check_version
check_docs
check_dirty
check_stripped
pack_loader_image
pack_trust_image
finish
