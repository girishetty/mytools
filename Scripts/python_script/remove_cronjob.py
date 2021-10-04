from crontab import CronTab

#Initialize Cron Tab
cron = CronTab(user='gshetty')
#Remove the job which was set up previously
cron.remove_all(command='python /Users/gshetty/work/codebase/repo/mytools/python_script/runme.py')

cron.write()
