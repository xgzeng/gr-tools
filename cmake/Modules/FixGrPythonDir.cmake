# don't use GR_PYTHON_DIR from GrPython module

execute_process(COMMAND ${PYTHON_EXECUTABLE} -c "
import sysconfig
print(sysconfig.get_config_var('py_version_short'))
" OUTPUT_VARIABLE PY_VERSION_SHORT OUTPUT_STRIP_TRAILING_WHITESPACE
)

# message(FATAL_ERROR ${GR_PYTHON_DIR} ${PY_VERSION_SHORT})
set(GR_PYTHON_DIR "lib/python${PY_VERSION_SHORT}/dist-packages")
