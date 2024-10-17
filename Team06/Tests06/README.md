# Systems Testing
## Instructions
1. Build the project, Tests06 will be copied to the build directory of autotester i.e
at the same directory as autotester.exe. 
2. Go to the directory which contains autotester.exe.
3. In the Tests06 folder, for each test folder, it contains a
.sh file which will run all the tests within that test folder and 
create an out folder which has the analysis.xsl and files generated
by the tests in that folder (if any)

The exception to step 3 is the Failure_Source folder. The intended
behavious when the script finish running is that no report should exist
in the out folder.
## File Structure

For system-testing, each high-level folder will focus on testing rigorously on the Source processing and the query processing individually. This method of testing is done due to
how errors in SP and errors in QPS are handled at the system-level i.e errors in processing the source code will immediately terminate without evaluating the provided queries, while
errors in one query will not terminate processing of further queries.
1. Failure_Source: Source files which are not meant to successfully be processed by SP. A dummy query file is present which is not meant to be processed by autotester.exe.
	- Folders:
		1. Failure_Invalid_Keywords
		2. Failure_Invalid_If_Structure
		3. Failure_Invalid_While_Structure
		4. Failure_Invalid_Statement_Structure
		5. Failure_Invalid_Lexical
		6. Failure_Invalid_Procedure_Structure
2. Success_Source: Source files which are meant to be successfully be processed by SP. A query file is present containing one simple query which is meant to be processed afterwards by autotester.exe.
	- Folders:
		1. Success_No_Nested
		2. Success_One_If
		3. Success_One_While
		4. Success_Two_Nested
		5. Success_Three_Nested
3. Failure_Queries: Queries which are meant to return either a SyntaxError or a SemanticError depending on the error present. 
Since the errors from queries is only dependently on the queries itself, a simple dummy source file suffices for each test case.
	- Files:
		1. Failure_Invalid_Keywords
		2. Failure_Invalid_Arguments
		3. Failure_Invalid_Declaration_Structure
		4. Failure_Invalid_Pattern_Structure
		5. Failure_Invalid_Such_That_Structure
4. Success_Queries: Queries which are meant to return an actual result. A source file of sufficient complexity will be present which will be suited to the specific tested query for each test case
	- Files:
		1. Success_One_Select
		2. Success_One_Such_That
		3. Success_One_Pattern
		4. Success_One_Such_That_One_Pattern
