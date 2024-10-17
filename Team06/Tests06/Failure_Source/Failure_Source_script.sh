mkdir -p out
cp ../analysis.xsl out/
for folder in */
do
    cd "$folder"
    for file in *_queries.txt
	  do
	     if [ "$(uname)" == "Darwin" ]
        then
            ../../../autotester "${file%_queries.txt}_source.txt" "$file" "../out/${file%_queries.txt}_out.xml"

        else
           ../../../autotester.exe  "${file%_queries.txt}_source.txt" "$file" "../out/${file%_queries.txt}_out.xml"
        fi
    done
    cd ..
done

cd ../out
python -m http.server 3000