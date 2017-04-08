ext=(
"'*.c'"
"'*.cpp'"
"'*.h'"
"'*.py'"
"'*.sh'"
)

exc_file=(
"'calculate.sh'"
"'__init__.py'"
)

exc_fold=(
"'./STM32*'"
)

exclude=""

for i in ${!exc_fold[@]}
do
    if [ "$exclude" ];then
        exclude="$exclude -or"
    fi
    exclude="$exclude -path ${exc_fold[$i]}"
done

for i in ${!exc_file[@]}
do
    if [ "$exclude" ];then
        exclude="$exclude -or"
    fi
    exclude="$exclude -name ${exc_file[$i]}"
done

if [ "$exclude" ];then
    exclude="! \( $exclude \) -and "
fi

extension=""
for i in ${!ext[@]}
do
    if [ "$extension" ];then
        extension="$extension -or"
    fi
    extension="$extension -name ${ext[$i]}"
done

if [ "$extension" ];then
    extension="\( $extension \)"
fi

args="$exclude$extension"

#echo "find . \( $args \) -print"
#echo `eval "find . \( $args \) -exec cat {} \; | wc -l"`
#echo "find . \( $args \) -exec awk -F# '\$0' {} \; | wc -l"

cmd0="find . \( $args \) -print"
cmd1="find . \( $args \) -exec cat {} \; | wc -l"
cmd2="find . \( $args \) -exec awk -F# '\$0' {} \; | wc -l"

eval $cmd0
lines=`eval $cmd1`
lines_useful=`eval $cmd2`
echo "总行数$lines"
echo "非空行$lines_useful"

read -p "Press enter to continue..." INP

# exclude blank lines
# find . -name "${data}"|xargs cat|grep -v ^$|wc -l

# exclude blank lines and begin with #
# find . -name "${ext[$i]}" -exec awk -F# '$1{print $1}' {} \; | wc -l

# exclude the lines begin with //
# find . -name "*.java"|xargs cat|grep -v -e ^$ -e ^\s*\/\/.*$|wc -l    #Output:36064

# find . -name "*.java"|xargs cat|wc -l
# find . -name "*.java"|xargs wc -l|grep "total"|awk '{print $1}'