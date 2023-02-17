while True :
    
    i = raw_input()
    
    if i == '0' :
        
        count = 1
        guess = input()

        # guess < 50 
        if guess < 50 :
            begin = 0
            end = 50            

            while True :
                answer = (begin+end)/2
            
                if answer > guess :     
                    end = answer
                    count = count + 1


                elif answer < guess :
                
                    begin = answer
                    count = count + 1

                
                elif answer == guess :
                    print (answer)
                    print (count)
                    count = 1
                    begin = 0
                    end = 100
                    break
                
        # guess > 50
        if guess > 50 :
            begin = 51
            end = 100            

            while True :
                answer = (begin+end)/2
            
                if answer > guess :     
                    end = answer
                    count = count + 1


                elif answer < guess :
                
                    begin = answer
                    count = count + 1


                elif answer == guess :
                    print (answer)
                    print (count)
                    count = 1
                    begin = 0
                    end = 100
                    break

        # guess == 50
        if guess == 50 :
            print (50)
            print (count)
            count = 1
            begin = 0
            end = 100
            
            



    if i == '1' :
        exit();
