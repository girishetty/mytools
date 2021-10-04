import sys
import datetime
import csv
from datetime import date
from datetime import timedelta
from ftplib import FTP

report_gen_day = date.today()

if len(sys.argv) > 1 :
    report_gen_day = datetime.datetime.strptime(sys.argv[1], "%m/%d/%Y").date()

diff = timedelta(days=6)
report_to_date = (report_gen_day - diff).strftime('%m/%d/%Y')
report_from_date = (report_gen_day - diff - diff).strftime('%m/%d/%Y')
print "Report Generation Date: " + str(report_gen_day)
print "Report From Date: " + report_from_date
print "Report To Date: " + report_to_date
if report_gen_day.month < 2 :
    year_date = str(report_gen_day.year - 1) + "12"
else :
    year_date = str(report_gen_day.year) + str(report_gen_day.month - 1).zfill(2)

print "Report YEARMONTH: " + year_date

fileName = "Report_" + str(report_gen_day.day) + "_" + str(report_gen_day.month) + "_" + str(report_gen_day.year) + ".csv"
print "FileName is: " + fileName
test_date1 = report_from_date.replace('/', '_')
test_date2 = report_to_date.replace('/', '_')
fileName = "Report_" + test_date1 + "_to_" + test_date2 + ".csv"
print "FileName is: " + fileName

#Now add your SQL code
#1. Open and Connect to SQL Server
#2. Prepare a cursor object
#3. Prepare your sql query, based on the from and to date
query = "select * from Table where sales_visit_date <= to_date('" + report_to_date + "', 'mm/dd/yyyy') AND sales_visit_date >= to_date('" + report_from_date + "', 'mm/dd/yyyy')"
print "Query is : " + query
#4. Execute your sql query
#cursor.execute(query)
#5. Process the sql query result
#cursor.execute("select * from tableName")

#csvFile = "tableName.csv"
#with open(csvFile, "wb") as csv_file:
#    csv_writer = csv.writer(csv_file)
#    #Write Table Headers first
#    csv_writer.writerow([i[0] for i in cursor.description])
#    # Now write all the rows
#    csv_writer.writerows(cursor)

#6. Disconnect from SQL Server

#Connect and transfer the csv file to the FTP Server
#ftp = FTP("server-name")
#ftp.login("username", "password")
#Change ftp server directory
#ftp.cwd("directory")
#ftp.storbinary("STOR " + csvFile, open(csvFile, "rb"), 1024)

