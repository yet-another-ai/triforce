include(MergeHeaders)

set(TRIFORCE_HEADERS
    cpu_detector.h
)

merge_headers("${TRIFORCE_DIST_DIR}/triforce.h" ${TRIFORCE_HEADERS})
