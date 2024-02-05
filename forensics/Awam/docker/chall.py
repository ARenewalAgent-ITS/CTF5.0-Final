flag1 = False
flag2 = False
flag3 = False
flag4 = False
flag5 = False

print("""
1. Apa nama email dari pengirim yang mengirimkan malware tersebut?
(Format: email@domain.tld)
    """)
answer1 = input(">>: ")
if answer1 == "malik.abdul28295@gmail.com":
    print('Correct!\n')
    flag1 = True
else:
    print('Wrong!\n')


print("""
2. Apa path lengkap dari malware yang didownload?
(Format: Drive:\\path\\to\\the\\file.ext)
    """)
answer2 = input(">>: ")
if answer2 == "C:\\Users\\adity\\Downloads\\gametest.zip":
    print('Correct!\n')
    flag2 = True
else:
    print('Wrong!\n')


print("""
3. Menurut antivirus yang ada, apa nama dari ancaman yang terdeteksi?
(Format: Type:Architecture/Name)

Ex: Backdoor:Win64/Caphaw.D!lnk
    """)
answer3 = input(">>: ")
if answer3 == "Trojan:Win32/Trickbot!ml":
    print('Correct!\n')
    flag3 = True
else:
    print('Wrong!\n')


print("""
4. Menurut artifact yang tersedia, kira-kira pada waktu berapa user menginisiasi malware ini? (UTC)
(Format: YYYY-MM-DD HH:MM)

Ex: 2024-01-11 11:11
    """)
answer4 = input(">>: ")
if answer4 == "2024-01-19 13:05":
    print('Correct!\n')
    flag4 = True
else:
    print('Wrong!\n')


print("""
5. Sepertinya password archive dikirimkan melalui medium yang berbeda. Apa password dari malware yang dikirimkan oleh pelaku tersebut? Format:
(password)

Ex: p4ssw0rd
    """)
answer5 = input(">>: ")
if answer5 == "infected":
    print('Correct!\n')
    flag5 = True
else:
    print('Wrong!\n')


if flag1 and flag2 and flag3 and flag4 and flag5:
    print("What?? Do you work as Digital Forensics Examiner? You're too good! Here's your flag:")
    print("ARA5{always_blame_the_user_when_an_incident_occurred_kappa}")
else:
    print('If you get this message but you got all answers correct, please contact admin and send the proof + each of your answers.')