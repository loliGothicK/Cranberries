PROGNAME=$(basename $0)
VERSION="0.1.0"

usage() {
    echo "Usage: $PROGNAME [OPTIONS] FILE"
    echo "  This script is ~."
    echo
    echo "Options:"
    echo "  -h, --help"
    echo "      --version"
    echo "  -a, --all"
    echo "  -target, --target"
    echo
    exit 1
}

FLAG_ALL=1

for OPT in "$@"
do
    case "$OPT" in
        '-h'|'--help' )
            usage
            exit 1
            ;;
        '--version' )
            echo $VERSION
            exit 1
            ;;
        '-all'|'--all' )
            shift 1
            ;;
        '-target'|'--target' )
            FLAG_ALL=0           
            FLAG_TARGET=1
            shift 1
            ;;
        '--'|'-' )
            shift 1
            param+=( "$@" )
            break
            ;;
        -*)
            echo "$PROGNAME: illegal option -- '$(echo $1 | sed 's/^-*//')'" 1>&2
            exit 1
            ;;
        *)
            if [[ ! -z "$1" ]] && [[ ! "$1" =~ ^-+ ]]; then
                #param=( ${param[@]} "$1" )
                param+=( "$1" )
                shift 1
            fi
            ;;
    esac
done

if [ "$FLAG_ALL" ]; then
  echo "Option -all specified."
  echo "add_subdirectory(test)" >> ./CMakeLists.txt
fi

if [ "$FLAG_TARGET" ]; then
  echo "Option -target specified."
  for i in ${param[@]}; do
  echo "add_subdirectory(test/$i)" >> ./CMakeLists.txt
  done
fi

cmake -DCMAKE_CXX_COMPILER=$CXX .. && make