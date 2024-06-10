# The version number.
set (SAGA_VERSION_MAJOR 9)
set (SAGA_VERSION_MINOR 4)
set (SAGA_VERSION_PATCH 2)

# get git commit hash (or dirty if git is not installed)

# load helper scripts
include(cmake/shared/load_git_variables.cmake)
include(cmake/shared/gen_version_string.cmake)

# load git variables
load_git_variables(GIT_BRANCH GIT_COUNT GIT_COMMIT)

# define version strings
gen_version_string(SAGA_VERSION_STRING SAGA_VERSION_FULL_STRING
	${SAGA_VERSION_MAJOR} ${SAGA_VERSION_MINOR} ${SAGA_VERSION_PATCH}
	${GIT_COUNT} ${GIT_COMMIT})

message("GIT_BRANCH:         ${GIT_BRANCH}")
message("VERSON_STRING:      ${SAGA_VERSION_STRING}")
message("VERSON_FULL_STRING: ${SAGA_VERSION_FULL_STRING}")
