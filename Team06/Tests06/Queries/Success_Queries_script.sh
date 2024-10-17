mkdir -p out
cp ../analysis.xsl out/
for file in *_queries.txt
do
   if [ "$(uname)" == "Darwin" ]
    then
      ../../autotester "Success_source.txt" "$file" "out/${file%_queries.txt}_out.xml"

    else
      ../../autotester.exe "Success_source.txt" "$file" "out/${file%_queries.txt}_out.xml"
    fi
done

cd out
python -m http.server 3000