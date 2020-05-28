from crontab import CronTab

#Initialize Cron Tab
cron = CronTab(user='gshetty')

print "Lines:"
for line in cron.lines:
    print line

print "Jobs:"
for job in cron:
    print job
