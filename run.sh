OUT="result/small_cases.txt"

touch ${OUT}

make gen_data
make main
make nguyen2020
make brute_force

for n in {12..12}
do
    echo "RUNNING" $n
    echo $n >> ${OUT}
    for i in {1..5}
    do
        echo "testing" $i
        build/gen_data $i $n 100000 5000000 30
        build/brute_force >> ${OUT}
        echo -n ' ' >> ${OUT}
        build/main >> ${OUT}
        echo -n ' ' >> ${OUT}
        build/nguyen2020 >> ${OUT}
        echo >> ${OUT}
    done
done
