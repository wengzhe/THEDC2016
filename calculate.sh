extensions=(
'*.c'
'*.h'
)

lines=0;
for data in ${extensions[@]}
do
    let lines=lines+`find . -name "${data}" -exec grep -n  ""  {} \; | wc -l`
done
echo $lines

read -p "Press enter to continue..." INP