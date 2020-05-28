from crontab import CronTab

#Initialize Cron Tab
cron = CronTab(user='gshetty')
#Add a new cron job
job = cron.new(command='python /Users/gshetty/work/codebase/repo/mytools/python_script/task.py')

#Set how oftern you want this job to be run
#job.minute.every(1)     # every minute
job.setall('1 7 * * 3')  # schedule this for every Wednesday at 7:01 am
job.enable()
cron.write()
