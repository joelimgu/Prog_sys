
question_a: 
	gcc question_a.c -o question_a -Wall -Wextra

intermediaire:
	gcc intermediaire.c -o intermediaire -Wall -pthread

lanceur:
	gcc intermediaire.c -o intermediaire -Wall -pthread && cc lanceur.c -o lanceur -Wall

fin:
	gcc intermediaire.c -o intermediaire -Wall -pthread && cc tete.c -o tete -Wall && gcc fin.c -o fin -Wall -pthread