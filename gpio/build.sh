NAME="defaultName"
PIBUILD="false"
DEBUG="-DDEFINE_DEBUG=OFF"
DEBUGENABLE="-DCMAKE_BUILD_TYPE=Debug -DDEFINE_DEBUG=ON"
DEPLOY=0
U=pi@raspberrypi.local
P=autochess

function checkQuit {
    if [ "$?" != 0 ];
    then
        echo "    > error occured"
        echo "    > terminating"
        exit
    fi
}

while getopts "dpn:" option; do
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

    esac
done
mkdir -p build
mkdir -p bin
cd build
echo '[  Running cmake  ]'
cmake $DEBUG -D PIBUILD:bool=$PIBUILD -D EXECNAME:string=$NAME --verbose=1  ..
checkQuit
echo '[  Running make  ]'
make
checkQuit
if [ $DEPLOY = 0 ];
then
    exit
fi
# echo "[  SCP $NAME to Pi  ]"
# sshpass -p $P $U:~ $PWD/$NAME
# checkQuit