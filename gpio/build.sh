NAME="defaultName"
PIBUILD="false"
DEBUG="-DDEFINE_DEBUG=OFF"
DEBUGENABLE="-DCMAKE_BUILD_TYPE=Debug -DDEFINE_DEBUG=ON"
MOVE=true
SCPU="pi@raspberrypi.local"

function checkQuit {
    if [ "$?" != 0 ];
    then
        echo "    > error occured"
        echo "    > terminating"
        exit 1
    fi
}

while getopts "dpmn:" option; do
    case "${option}" in
        n)
        NAME=${OPTARG}
        ;;
        p)
        PIBUILD="true"
        ;;
        d)
        DEBUG=$DEBUGENABLE
        ;;
        m)
        MOVE=true
        ;;
    esac
done

if [ "$PIBUILD" = "false" ];
then
    NAME="${NAME}_local"
fi

mkdir -p build
mkdir -p bin
cd build
echo '[  Running cmake  ]'
cmake $DEBUG -D PIBUILD:bool=$PIBUILD -D EXECNAME:string=$NAME --verbose=1  ..
checkQuit
echo '[  Running make  ]'
make
checkQuit
if [ $MOVE = 0 ];
then
    exit 1
fi
echo "[  SCP ${NAME} to Pi  ]"
if [ "$PIBUILD" == "false" ];
then
    echo "    > Will not push local build to pi"
    echo "    > terminating"
    exit 1
fi
cd ..
sshpass -f ../sshpass scp bin/$NAME $SCPU:~/$NAME
# checkQuit