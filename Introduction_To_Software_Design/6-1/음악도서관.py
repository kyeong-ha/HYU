# -*- coding: utf-8 -*-

# 음악 검색 기능
def musicSerch() :
    if  musicData == [] :
        print '현재 등록된 음악이 없습니다. 음악을 추가하시겠습니까? (yes 또는 no)'
        add_input = raw_input()

        while True :
            if add_input == 'yes' :
                musicAdd()
                break

            elif add_input == 'no' :
                print """초기화면으로 돌아갑니다.
                """
                break

            else :
                print "yes 또는 no를 입력해주세요."
                add_input = raw_input()
    else :
        print "(1) 제목검색 (2) 가수검색 (3) 장르검색 (4) 평점검색"
        serch_input = raw_input()
        
        while True :
            if serch_input == '1' :
                print "찾으시는 노래 제목이 무엇인가요?"
                name = raw_input("제목 : ")
                for i in range(0, len(musicData)) :
                    if musicData[i][0] == name :
                        print musicData[i]
                print
                        
                break
                
            elif serch_input == '2' :
                print "찾으시는 가수 이름이 무엇인가요?"
                artist = raw_input("가수 : ")
                for i in range(0, len(musicData)) :
                    if musicData[i][1] == artist :
                        print musicData[i]
                print
                                  
                break
                        
            elif serch_input == '3' :
                print "찾으시는 장르가 무엇인가요?"
                print "(1. ballad 2. rock 3. r&b 4. acusic)"
                serch_ganre = raw_input()
                
                while True :
                    
                    if serch_ganre == '1' :
                        for i in range(0, len(musicData)) :
                            if musicData[i][2] == 'ballad' :
                                print musicData[i]
                        print
                                
                        break
                            
                            
                            
                    elif serch_ganre == '2' :
                        for i in range(0, len(musicData)) :
                            if musicData[i][2] == 'rock' :
                                print musicData[i]
                        print   
                        break

                    elif serch_ganre == '3' :
                        for i in range(0, len(musicData)) :
                            if musicData[i][2] == 'r&b' :
                                print musicData[i]
                        print
                        break

                    elif serch_ganre == '4' :
                        for i in range(0, len(musicData)) :
                            if musicData[i][2] == 'acustic' :
                                print musicData[i]
                        print
                        break

                    else :
                        print "1 또는 2 또는 3 또는 4를 입력해주세요."
                        serch_ganre = raw_input()
                break

            elif serch_input == '4' :
                print "찾으시는 평점은 몇 점인가요?"
                score = raw_input("평점 : ")
                for i in range(0, len(musicData)) :
                    if musicData[i][3] <= score :
                        print musicData[i]
                print              
                break

            else :
                print "1 또는 2 또는 3 또는 4를 입력해주세요."
                serch_input = raw_input()

# 음악 추가 기능
def musicAdd() : 
    print "추가하려는 음악의 정보를 입력해주세요." 
    name = raw_input("제목 : ")
    artist = raw_input("가수 : ")
    ganre = raw_input("장르 (balad, rock, r&b, acustic 중 택 1) : ")
    score = raw_input("평점 : ")
    description = raw_input ("설명 : ")
        
    print "입력하신 정보가 맞나요? (yes or no)"
    print ("[" + name + ", " + artist + ", " + ganre + ", " + score + ", " + description + "]")
    check_input = raw_input()

    while True :
        if check_input == 'yes' :
            if musicData == [] :
                musicData.append([name, artist, ganre, score, description])
                print """입력하신 음악이 추가되었습니다! 초기 화면으로 돌아갑니다.
"""
                break

            else :
                for i in range(0, len(musicData)) :
                    if musicData[i][0:4] == [name, artist, ganre, score] :
                        print "입력한 음악은 이미 존재합니다. 초기 화면으로 돌아갑니다."
                        print
                        break

                    else :  
                        musicData.append([name, artist, ganre, score, description])
                        print """입력하신 음악이 추가되었습니다! 초기 화면으로 돌아갑니다.
"""
                        break
                break

        elif check_input == 'no' :
            print """초기화면으로 돌아갑니다.
"""
            break

        else :
            print "yes 또는 no를 입력해주세요."
            check_input = raw_input()
    
# 음악 제거  기능
def musicDel() :
    global musicData
    if musicData == [] :
        print '현재 등록된 음악이 없습니다. 음악을 추가하시겠습니까? (yes 또는 no)'
        add_input = raw_input()

        while True :
            if add_input == 'yes' :
                musicAdd()
                break

            elif add_input == 'no' :
                print """초기화면으로 돌아갑니다.
                """
                break

            else :
                print "yes 또는 no를 입력해주세요."
                add_input = raw_input()
            

    else :
        print """1. 모든 음악 초기화
2. 1개의 음악 제거
3. 돌아가기"""
        del_input = raw_input()

        while True :
            if del_input == '1' :
                musicData = [] 
                print """모든 음악이 제거되었습니다! 초기 화면으로 돌아갑니다.
            """
                break

            elif del_input == '2' :
                print "제거하려는 음악의 정보를 입력해주세요."
                name = raw_input("제목 : ")
                artist = raw_input("가수 : ")
                ganre = raw_input("장르(balad, rock, r&b, acustic 중 택 1) : ")
                score = raw_input("평점 : ")
                
            
                for i in range(0,len(musicData)) :
                    if musicData[i][0:4] == [name, artist, ganre, score] :
                        del musicData[i]
                        print """입력하신 음악이 제거되었습니다! 초기 화면으로 돌아갑니다.
"""
                        break
                else :
                    print """입력하신 음악은 존재하지 않습니다. 초기 화면으로 돌아갑니다.
"""
                break                    
                
            elif del_input == '3' :
                print """초기화면으로 돌아갑니다.
            """      
        
            else :
                print "'1' 또는 '2' 또는 '3' 을 입력해주세요."
                del_input = raw_input()


        
        
def musicChange () : 
    if musicData == [] :
        print '현재 등록된 음악이 없습니다. 음악을 추가하시겠습니까? (yes 또는 no)'
        add_input = raw_input()

        while True :
            if add_input == 'yes' :
                musicAdd()
                break

            elif add_input == 'no' :
                print """초기화면으로 돌아갑니다.
                """
                break

            else :
                print "yes 또는 no를 입력해주세요."
                add_input = raw_input()

    else :
        print "등록된 음악의 정보를 교체하시겠습니까? (yes or no)"
        change_input = raw_input()
    
        while True :
                if change_input == 'yes' : 
                    print "교체하려는 음악의 정보를 입력해주세요."
                    name = raw_input("제목 : ")
                    artist = raw_input("가수 : ")
                    ganre = raw_input("장르(balad, rock, r&b, acustic 중 택 1) : ")
                    score = raw_input("평점 : ")
                    for i in range(0, len(musicData)) :
                        if musicData[i][0:4] == [name, artist, ganre, score] :
                            print "어떤 음악으로 교체하시겠습니까?"
                            print ("현재 음악 정보 : " + ", ".join(musicData[i]))
                            nameChange = raw_input("제목 : ")
                            artistChange = raw_input("가수 : ")
                            ganreChange = raw_input("장르(balad, rock, r&b, acustic 중 택 1) : ")
                            scoreChange = raw_input("평점 : ")
                            descriptionChange = raw_input("설명 : ")
                            for i in range(0, len(musicData)) :
                                if musicData[i][0:4] == [nameChange, artistChange, ganreChange, scoreChange] :
                                    print "입력한 음악은 이미 존재합니다. 초기 화면으로 돌아갑니다."
                                    print
                                    break
        
                            else :
                                musicData[i] = [nameChange, artistChange, ganreChange, scoreChange, descriptionChange]
                                print musicData[i]
                                print "음악 교체가 완료되었습니다!"
                                print
                                break
                            break
                            

                    else :
                        print """입력하신 음악은 존재하지 않습니다. 초기 화면으로 돌아갑니다.
"""
                        break
                    break
                    

                elif change_input == 'no' :
                    print """초기화면으로 돌아갑니다.
"""
                    break

                else :
                    print "yes 또는 no를 입력해주세요."
                    change_input = raw_input()
                

def musicSort() :
    if musicData== [] :
        print '현재 등록된 음악이 없습니다. 음악을 추가하시겠습니까? (yes 또는 no)'
        add_input = raw_input()

        while True :
            if add_input == 'yes' :
                musicAdd()
                break

            elif add_input == 'no' :
                print """초기화면으로 돌아갑니다.
                """
                break

            else :
                print "yes 또는 no를 입력해주세요."
                add_input = raw_input()
    else :
        print "어떤 기준으로 정렬하실건가요?"
        print "① 제목 ② 가수 ③ 평점"
        Sort_input = raw_input()

        while True :
            if Sort_input == '1' :
                print "① 오름차순 ② 내림차순"
                sortChoice = raw_input()
                while True :
                    if sortChoice == '1' :
                        print "[ 제목, 가수, 장르, 평점, 설명 ]"
                        musicData.sort()
                        for i in range(0, len(musicData)) :
                            print (musicData[i])
                        print
                        break

                    elif sortChoice == '2' :
                        print "[ 제목, 가수, 장르, 평점, 설명 ]"
                        musicData.sort(reverse=True)
                        for i in range(0, len(musicData)) :
                            print (musicData[i])
                        print
                        break            

                    else :
                        print "1 또는 2를 입력해주세요."
                        sortChoice == raw_input()
                break
                
            elif Sort_input == '2' :
                print "① 오름차순 ② 내림차순"
                sortChoice = raw_input()
                while True :
                    if sortChoice == '1' :
                        print "[ 제목, 가수, 장르, 평점, 설명 ]"
                        musicData.sort(key=lambda x:x[1])
                        for i in range(0, len(musicData)) :
                            print (musicData[i])
                        print
                        break

                    elif sortChoice == '2' :
                        print "[ 제목, 가수, 장르, 평점, 설명 ]"
                        musicData.sort(reverse=True, key=lambda x:x[1])
                        for i in range(0, len(musicData)) :
                            print (musicData[i])
                        print
                        break            

                    else :
                        print "1 또는 2를 입력해주세요."
                        sortChoice == raw_input()
        

            elif Sort_input == '3' :
                print "① 평점높은순 ② 평점낮은순"
                sortChoice = raw_input()
                while True :
                    if sortChoice == '1' :
                        print "[ 제목, 가수, 장르, 평점, 설명 ]"
                        musicData.sort(key=lambda x:x[3])
                        for i in range(0, len(musicData)) :
                            print (musicData[i])
                        print
                        break

                    elif sortChoice == '2' :
                        print "[ 제목, 가수, 장르, 평점, 설명 ]"
                        musicData.sort(reverse=True, key=lambda x:x[3])
                        for i in range(0, len(musicData)) :
                            print (musicData[i])
                        print
                        break            

                    else :
                        print "1 또는 2를 입력해주세요."
                        sortChoice == raw_input()
                        
            else :
                print "1 또는 2 또는 3를 입력해주세요."
                Sort_input = raw_input()
            break
# Main
musicData = []

while True :
    print "음악도서관에 오신 걸 환영합니다! 무엇을 도와드릴까요?"
    print "(1) 음악 검색 (2) 음악 추가 (3) 음악 제거 (4) 음악 교체 (5) 음악 정렬 (0) 종료"
 
    choice_input = raw_input()

    if choice_input == '1' :
        musicSerch()

    elif choice_input == '2' :
        musicAdd()

    elif choice_input == '3' :
        musicDel()
        
    elif choice_input == '4' :
        musicChange()

    elif choice_input == '5' :
        musicSort() 

    elif choice_input == '0' :
        exit()
