VERS="504 606 610 615 620"

TESTS=$(cat st_list.txt)

rm -f *.csv
echo -n "versions " >st.csv
for v in $VERS; do
    echo -n ", v${v}" >>st.csv
done
echo >>st.csv

for t in $TESTS; do
    for v in $VERS; do
	make -f st${t}.mk
	./st.sh -v $v $t
    done
    (
	echo -n "st${t} "
	for v in $VERS; do
	     echo -n ", $(cat st${t}${v}.txt) "
	done
	echo
    ) >st${t}.csv
    cat st${t}.csv >>st.csv
done

