@echo off
setlocal EnableDelayedExpansion
set data=3


echo: 

set /p serverName= "Please input server name / (default : '.') enter 0 to skip :  " 

IF  "!serverName!" == "0" (
set serverName=.
)

set /p dbUser=  "Please input db user name :  " 
set /p dbPassword=  "Please input db password :  " 

echo Logging in....
sqlcmd -U %dbUser% -P %dbPassword% -S %serverName% -Q ""

echo:
echo Logged In Successfully, Welcome %dbUser%


echo:


set counter=%~1
set databaseName=%~2

for /l %%x in (1, 1, !counter!) do (
	SET /p indexName= "please enter index name: "
	SET /p tableName= "please enter tableName: "
	SET /p indexColumn= "please enter index column name: "
    sqlcmd -U !dbUser! -P !dbPassword! -S !serverName! -d !databaseName! -Q "CREATE INDEX !indexName! ON !tableName! (!indexColumn!)"  -b 
	echo CREATED INDEX Successfully
	echo:
)


SET /p tableName= "press any key to continue..."