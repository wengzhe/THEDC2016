extensions=(
'*.c'
'*.h'
)

lines=0;
lines_useful=0;
for data in ${extensions[@]}
do
    let lines=lines+`find . -name "${data}" -exec grep -n "" {} \; | wc -l`
    let lines_useful=lines_useful+`find . -name "${data}" -exec grep -v ^$ {} \; | wc -l`
done
echo "总行数$lines"
echo "非空行$lines_useful"

read -p "Press enter to continue..." INP

# exclude blank lines
# find . -name "${data}"|xargs cat|grep -v ^$|wc -l

# exclude the lines begin with //
# find . -name "*.java"|xargs cat|grep -v -e ^$ -e ^\s*\/\/.*$|wc -l    #Output:36064

# find . -name "*.java"|xargs cat|wc -l
# find . -name "*.java"|xargs wc -l|grep "total"|awk '{print $1}'