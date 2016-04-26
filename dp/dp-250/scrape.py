import requests

class dppost:
  difficulty = ''
  title = ''
  permalink = ''
  num = 'asdf'

  def print_post(self):
    s = ''
    s+='['+self.title+']('+self.permalink+')|'
    return s 

class dpweek:
  number = 'asdf'
  posts = []

  def print_week(self):
    s = '|'
    for p in self.posts:
      if('Easy' in p.difficulty and p.num == self.number):
        s+=p.print_post()
    for p in self.posts:
      if('Intermediate' in p.difficulty and p.num == self.number):
        s+=p.print_post()
    for p in self.posts:
      if('Hard' in p.difficulty and p.num == self.number):
        s+=p.print_post()
    for p in self.posts:
      if('Weekly' in p.difficulty and p.num == self.number):
        s+=p.print_post()
    return s

base='https://api.reddit.com/r/dailyprogrammer/?raw_json=1'
after=''
headers = {
  'User-Agent': 'test'
}

print('Easy | Intermediate | Hard | Weekly/Bonus')
print('-----|--------------|------|-------------')
i=0
weeks = {}
week = dpweek()
nums = []
while after != None:
#  i+=1
#  if(i>3):
#    break;
  r=requests.get(base+'&after='+after,headers=headers)

  obj = r.json()
#print(obj)
  for post in obj['data']['children']:
    p = dppost()
    p.title = post['data']['title']
    title = p.title
    if('Challenge' not in title):
      continue
    #print(title)
    numstart = title.find('Challenge #')+len('Challenge #')
    if(numstart != len('Challenge #') -1):
      numend = title.find(' ',numstart)
      if(numend == numstart):
        numend=title.find(' ',numend+1)
      diffstart = title.find('[',numend)
      diffend = title.find(']',diffstart)
      p.permalink = post['data']['permalink']
      p.difficulty = title[diffstart+1:diffend]
    else:
      p.difficulty = 'Weekly'
#not a normal daily 
    number = title[numstart:numend].strip()
    if(number not in weeks):
#couldn't find this weeks number, make a new row
      week = dpweek()
      week.number = number
      weeks[week.number] = week
      nums.append(week.number)
    p.num = week.number
    weeks[number].posts.append(p)

  after = obj['data']['after']

for week in nums:
#  print(week.number)
  print(weeks[week].print_week().encode('utf_8'))
