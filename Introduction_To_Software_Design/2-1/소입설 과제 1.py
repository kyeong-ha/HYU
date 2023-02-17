print 'mode 0 : continue, 1 : exit'

a = raw_input();

if a == '0':
    while True :
        print 'Hello! What is your name?'
        name = raw_input()
        print 'Your name is ' + name
        print 'StudentNumber :'
        studentNumber = raw_input()
        print 'PhoneNumber :'
        phoneNumber = raw_input()
        print 'University :'
        university = raw_input()
        print name + "'s Information"
        print university + ' ' + studentNumber
        print 'PhoneNumber : ' +  phoneNumber
        print 'mode 0 : continue, 1 : exit'
        if raw_input() == '1' :
           exit();

if a == '1':
    exit();

    
