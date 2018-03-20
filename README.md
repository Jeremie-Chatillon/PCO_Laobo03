#PCO Labo03: Mandelbrot

##### Châtillon, Smith

Affin de réaliser ce laboratoire, nous avois dû extraire une partie du code de RenderThread dans une autre classe que nous avons céée. Afin de pouvoir correctement mulit-hreader, il falait que tous les threads enfants appliquent le même code mais avec des variables differentes. Nous avons donc choisi de créer une casse DelegationThread qui effectue les calculs pour une partie de l'image. Cette classe à pour but de calculer les pixels de l'image sur une proprtion donnée (dans notre cas nous avons sépraré l'image en plusieurs sous-image avec sa hauteur).  Comme la divions de la hauteur se fair avec un division entière, le dérnier DelegationThread travaille avec un auteur un peu plus grande que les autres car on y ajout le reste de la division (our ne pas avoir de lignes noires à la fin de l'image). Tous les argument qui ne sont pas des types primitifs sont passé par pointeurs pour ne pas faire une copie inutil de ceux-ci. Attention, que l'image peut et doit être modifiée. Les autres arg

Ainsi, la méthode run() de la classe délégation thread ne contient seulement la boucle qui effectue le nombre de passe. Dans cette boule, à chaque pars, n (en fonction  du retour de QThread::idealThreadCount()) DelegationTread sont instancié puis executé(.run()). Ils sont stoqué dans une liste. Une fois les DelegationTreads lancés, la boucle attends que ces threads aient térimné leur job (.wait()) et les supprime (.delete()). Une fois ces actions effecuées, la boucle recommence avec un nombre de pars plus grand. Le RenderThread n'effectue aucune action avant la terminaison de chacun de ses threads enfant.

Il est possible que l'utilisateur change, arrête l'execution du thread principal (redimentionnement de la fenêtre par exemple). Pour que les DelegationTreads ne continuent pas leur tâche, nous avons fait passer les variables abort et restart par pointeur pour que les ceux-ci se términenent au moment voulus. 

Il n'est pas nécessaire d'implémenter un déstructeur spécifique dans la classe DelegationTread. Ceci est dû au fait que la fonction RenderThread attends la fin de l'execution de sa fonction run() avant de pouvoir être supprimée. Donc les DelegationTreads lancé à l'intérieurs seront aussi términés correctement.



### Tests

Pour tester notre programme, nous avons verifié que l'image obtenue à l'execution était conforme et redimentionnable.

![Mandelbrot](C:\Users\Jeremie\Documents\HEIG\PCO\Labo\Labo03.1\PCO_Laobo03\Mandelbrot.PNG)

Nous avons aussi tésté que le nombre de passe se réinitialise à chacune des modification de l'image(interruption des calculs).

![Mandelbrot_abort](C:\Users\Jeremie\Documents\HEIG\PCO\Labo\Labo03.1\PCO_Laobo03\Mandelbrot_abort.PNG)



Finalement, nous avons contrôlé que le multithreading était bien actif en contrôlant l'état de chacun des coeurs de notre processeur pendant l'execution.![Result](C:\Users\Jeremie\Documents\HEIG\PCO\Labo\Labo03.1\Result.jpg)