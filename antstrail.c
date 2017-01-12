/*
 *
 * ANT SIMULATION @ RYU FUJIWARA
 * LAST EDITED: Jan 25, 2015
 * VERSION 1.0
 * 
 * This program simulates ants' behavior of
 * creating trails by their communicating 
 * with each other via 2 types of pheromones.
 * When you run this program, you will see
 * three boxes on top and three bars at bottom.
 * 
 * The Left box shows the ants' movement.
 * The middle and the right one will show the 
 * distribution of eventual distributions of
 * two pheromones after the simulation is done.
 * 
 * The bars at the bottom represents the progress
 * of overall simulation, ratio of three modes
 * of ants alive, and the probability that ants
 * came back so far from top to bottom.
 * 
 * When all the procedures end, this program
 * will spit out a csv file that shows stores 
 * information of the setting, and the probability
 * of ants' coming back to the nest.
 * 
 * Enjoy seeing the process that ants create
 * their trail!!
 *
 *
 */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <glsc.h>
# include <time.h>

/*
 *
 * COMPOSITION
 *
 */

/* ant */
typedef struct {
	/* coordinates in hex */ 
	int x;
	int y;

	/* where it is looking at: 0 ~ 5 */
	/* 0 is up, and 1 ~ is defined clockwisely */
	int direction;

	/* moving mode: 1 ~ 3? */
	int mode;

	/* holds vitality */
	int vitality;

	/* threshold of pheromone detection */
	int threshold[2];

	/* whether this ant has found food or not */
	int hasFood;

	/* whether this ant has come back or not */
	int hasComeBack;
} ant;

/*
 *
 * DEFINE
 *
 */
/*
# define L_MAX (50)
# define ANT_NUM (1000)
# define N (150)
# define FOOD_NUM (3)
# define SURROUNDING_MODE (0)
# define OBSTACLE_NUM (10)
# define OBSTACLE_SURROUNDING (1)
# define OBSTACLE_MODE (0)
# define MAX_PROCEDURE (N * 2000)
# define FPS (0)
# define LUG (N / (double)ANT_NUM)
# define THRESHOLD_ONE_SENSITIVE (40.0)
# define THRESHOLD_ONE_INSENSITIVE (80.0)
# define THRESHOLD_TWO_SENSITIVE (15.0)
# define THRESHOLD_TWO_INSENSITIVE (30.0)
# define PHEROMONE_RELEASE_ONE (2.0)
# define PHEROMONE_RELEASE_TWO (1.0)
# define INCREMENT (1)
# define DIFFUSION_COEF_ONE (0.000001 / 6.0)
# define EVAPORATE_COEF_ONE (0.97)
# define DIFFUSION_COEF_TWO (0.000001 / 6.0)
# define EVAPORATE_COEF_TWO (0.999)
# define SET_SEED (-1)
# define DOES_SET_FOOD (0)
 /*

/* IT WORKS */

# define L_MAX (30)
# define ANT_NUM (50)
# define N (200)
# define FOOD_NUM (3)
# define SURROUNDING_MODE (0)
# define OBSTACLE_NUM (10);
# define OBSTACLE_SURROUNDING (1)
# define OBSTACLE_MODE (0)
# define MAX_PROCEDURE (N * 100)
# define FPS (0)
# define LUG (N / (double)ANT_NUM)
# define THRESHOLD_ONE_SENSITIVE (40.0)
# define THRESHOLD_ONE_INSENSITIVE (80.0)
# define THRESHOLD_TWO_SENSITIVE (15.0)
# define THRESHOLD_TWO_INSENSITIVE (30.0)
# define PHEROMONE_RELEASE_ONE (2.0)
# define PHEROMONE_RELEASE_TWO (1.0)
# define INCREMENT (1)
# define DIFFUSION_COEF_ONE (0.000001 / 6.0)
# define EVAPORATE_COEF_ONE (0.997)
# define DIFFUSION_COEF_TWO (0.000001 / 6.0)
# define EVAPORATE_COEF_TWO (0.999)
# define SET_SEED (-1)
# define DOES_SET_FOOD (0)

 /* */

/*
 *
 * GLOBAL VARIABLES
 *
 */

/* stores actual xy coordinates of hex - coordinate */
double coordinate[L_MAX * 2 + 1][L_MAX * 2 + 1][2];

/* Pheromone */
double pheromone[L_MAX * 2 + 1][L_MAX * 2 + 1][2];
double tempPheromone[L_MAX * 2 + 1][L_MAX * 2 + 1][2];
double normalizedPheromoneOne[L_MAX * 2 + 1][L_MAX * 2 + 1];
double normalizedPheromoneTwo[L_MAX * 2 + 1][L_MAX * 2 + 1];

/* NOT USED */
int obstaclePlace[L_MAX * 2 + 1][L_MAX * 2 + 1];
int foodPos[FOOD_NUM * 7][2];

/* statistics */
int comeBack;
int totalAnt = 0;
int procedureCount = 0;
int ratio[3];
double homingProb[MAX_PROCEDURE];
int homingNum[MAX_PROCEDURE];
int startingTime;
double remainingTime;

/* Ants */
ant antList[ANT_NUM];

/* Seed */
int seed;


/*
 *
 * PROTOTYPE
 *
 */

/* Initializer and halting */
void initialization ();
void honeyCombInitialization ();
void graphicInitialization ();
void instructionInitialization ();
void boxExplanation ();
void pheromoneInitialization ();
void antInitialization();
void initializeGivenAnt (int a_n);
void foodInitialization ();
void backgroundInitialization ();
void calculateRemaningTime ();
void drawAllFood ();
void drawNest ();
void halt();

/* Graphic */
void progressBar ();
void ratioBar ();
void homingBar ();
void normalizePheromone ();
void drawPheromoneDistribution ();

/* Ant behavior */
void singleAntMovement (int a_n);
void singleMovement (int a_n);
void multipleMovement ();
void consecutiveMovement ();
void movementOne (int a_n);
void movementTwo (int a_n);
void movementThree (int a_n);
int hasDiscoveredFood (int a_n);
int hasGoneBack (int a_n);
int calculateDirection (int mode, int dir, int a_n);
int calculateProb (int tempPheromone[6], int dir);
void moveToGivenDirection (int dir, int a_n);
void plotAnt (int a_n, int color);
int checkPheromone (int a_n, int pheromone_num);

/* Pheromone Behavior */
void pheromoneBehavior ();
void pheromoneDiffusion ();
void pheormoneEvaporation ();

/* File output */
void createAndWriteInFile ();

/* Test */
void graphicTest ();
void structAntTest (int a_n);

/*
 *
 * FUNCTIONS
 *
 */


/* Main */
int main () {
	initialization();

	consecutiveMovement();
	//multipleMovement();
	//singleAntMovement(0);

	printf ("%d / %d\n", comeBack, totalAnt);

	drawPheromoneDistribution ();
	halt();

	return 0;
}

/* -------------------- INITIALIZATION AND HALT ------------------------------ */

/* Called in the beginning */
void initialization () {
	if (SET_SEED >= 0) {
		seed = SET_SEED;
	}
	else {
		seed = (unsigned) time(NULL);
	}

	startingTime = (unsigned) time (NULL);
	
	srand(seed);

	honeyCombInitialization();
	pheromoneInitialization();
	foodInitialization();
	antInitialization();
	graphicInitialization();
	backgroundInitialization();
	instructionInitialization();
	boxExplanation ();
}

/* Graphic initializer */
void graphicInitialization () {
	g_init("GRAPH", 300.0, 120.0);
	g_device (G_DISP);
}

/* BackGround initializer */
void backgroundInitialization () {
	/* background */
	g_area_color(g_rgb_color(0.5, 0.5, 0.5));
	g_box(0, 300, 0, 120, 0, 1);

	/* for ant movement */
	g_def_scale (1, -L_MAX * 1.1, L_MAX * 1.1, -L_MAX * 1.1, L_MAX * 1.1, 10.0, 10.0, 80.0, 80.0);
	/* for progress bar */
	g_def_scale (2, 0, MAX_PROCEDURE, 0, 10, 40, 92.5, 240, 5);
	/* for ant mode ratio */
	g_def_scale (3, 0, ANT_NUM, 0, 10, 40, 102.5, 240, 5);
	/* for homing ratio bar */
	g_def_scale (4, 0, 1, 0, 10, 40, 112.5, 240, 5);
	/* second and third box */
	g_def_scale (5, -1.1, 1.1, -1.1, 1.1, 110.0, 10.0, 80.0, 80.0);
	g_def_scale (6, -1.1, 1.1, -1.1, 1.1, 210.0, 10.0, 80.0, 80.0);


	/* background of pheromone */
	g_area_color(g_rgb_color(0.2, 0.2, 0.2));
	g_line_color (1);
	g_box (110, 190, 10, 90, 1, 1);
	g_box (210, 290, 10, 90, 1, 1);

	/* background of ants */
	g_sel_scale (1);
	g_area_color (0);
	g_box (-L_MAX * 1.1, L_MAX * 1.1, -L_MAX * 1.1, L_MAX * 1.1, 1, 1);

	/* background of progress bar */
	g_sel_scale (2);
	g_line_color (0);
	g_area_color(g_rgb_color(0.2, 0.2, 0.2));
	g_box (0, MAX_PROCEDURE, 0, 10, 1, 1);

	/* background of ratio bar */
	g_sel_scale (3);
	g_box (0, ANT_NUM, 0, 10, 1, 1);

	/* background of homing ratio bar */
	g_sel_scale (4);
	g_box (0, 1, 0, 10, 1, 1);

	/* just forgetting g_rgb_color (unwilling but inevitable...) */
	g_area_color (0);
	g_line_color (0);
}

/* Instruction initialization */
void instructionInitialization () {
	char firstLine[256];
	char secondLine[256];
	char thirdLine[256];
	char seedLine[256];

	/* first line */
	char l[16] = "L";
	char antnum[20] = "Ant Num";
	char n[16] = "Vitality";
	char surrounding[20] = "Surrounding Mode";
	char maxP[16] = "Max Procedure";

	sprintf (firstLine, "%s = %d, %s = %d, %s = %d, %s = %d, %s = %d", l, L_MAX, antnum, ANT_NUM, n, N, surrounding, SURROUNDING_MODE, maxP, MAX_PROCEDURE);

	/* second line */
	char t[16] = "Threshold";
	char pherRel[20] = "Pheromone release";

	sprintf (secondLine, "%s (Sensitive: %3.1lf, Insensitive %3.1lf), %s (One: %3.1lf, Two: %3.1lf)", t, THRESHOLD_TWO_SENSITIVE, THRESHOLD_TWO_INSENSITIVE, pherRel, PHEROMONE_RELEASE_ONE, PHEROMONE_RELEASE_TWO);

	/* third line */
	char diff[16] = "Diffusion";
	char evap[16] = "Evaporation";

	sprintf (thirdLine, "%s (One: %lf, Two: %lf), %s (One: %lf, Two: %lf)", diff, DIFFUSION_COEF_ONE, DIFFUSION_COEF_TWO, evap, EVAPORATE_COEF_ONE, EVAPORATE_COEF_TWO);

	g_text_font (1);
	g_text_color (7);

	g_text (10, 3, firstLine);
	g_text (10, 6, secondLine);
	g_text (10, 9, thirdLine);

	g_text (4, 96.5, "PROGRESS BAR");
	g_text (4, 106.5, "MODE RATIO");
	g_text (4, 116.5, "HOMING PROBABILITY");

	/* write seed */
	sprintf (seedLine, "SEED = %d", seed);
	g_text (260, 3, seedLine);
}

/* Explains each box */
void boxExplanation () {
	/* First box */
	g_sel_scale (1);

	g_area_color (g_rgb_color(0.2, 0.2, 0.2));
	g_line_color (7);

	g_box (-L_MAX, -0.5 * L_MAX, L_MAX, 0.8 * L_MAX, 1, 1);

	g_text_color (7);
	g_text (18.5, 17, "ANTS");
	g_text (14.5, 20, "MODE");

	g_text_color (7);
	g_text (24.5, 20, "1");
	g_text_color (1);
	g_text (26.5, 20, "2");
	g_text_color (2);
	g_text (28.5, 20, "3");

	/* Second box */
	g_area_color(0);
	g_sel_scale (5);
	g_box (-1, -0.35, 1, 0.8, 1, 1);
	g_sel_scale (6);
	g_box (-1, -0.35, 1, 0.8, 1, 1);

	g_text_color (7);
	g_text (114.5, 17, "PHEROMONE 1");
	g_text (114.5, 20, "DISTRIBUTION");

	g_text (214.5, 17, "PHEROMONE 2");
	g_text (214.5, 20, "DISTRIBUTION");
}

/* Initialize honey comb: Maps Hex to xy coordinate */
void honeyCombInitialization () {
	int i, j;
	float x = sqrt(3) / 2.0;
	float y = 1 / 2.0;

	for (i = 0; i < L_MAX * 2 + 1; i++) {
		for (j = 0; j < L_MAX * 2 + 1; j++) {
			coordinate[i][j][0] = 0;
			coordinate[i][j][1] = 0;
		}
	}

	/* (0, 0) is (L_MAX, L_MAX) */
	for (i = 1; i <= L_MAX; i++) {
		/* top and bottom */
		coordinate[L_MAX - i][L_MAX][0] = 0;
		coordinate[L_MAX - i][L_MAX][1] = i;

		coordinate[L_MAX + i][L_MAX][0] = 0;
		coordinate[L_MAX + i][L_MAX][1] = -i;

		for (j = 1; j <= i; j++) {
			coordinate[L_MAX - i][L_MAX + j][0] = coordinate[L_MAX - i][L_MAX + j - 1][0] + x;
			coordinate[L_MAX - i][L_MAX + j][1] = coordinate[L_MAX - i][L_MAX + j - 1][1] - y;

			coordinate[L_MAX + i][L_MAX - j][0] = coordinate[L_MAX + i][L_MAX - j + 1][0] - x;
			coordinate[L_MAX + i][L_MAX - j][1] = coordinate[L_MAX + i][L_MAX - j + 1][1] + y;
		}

		/* right and left */
		for (j = i - 1; j >= 0; j--) {
			coordinate[L_MAX - j][L_MAX + i][0] = coordinate[L_MAX - j - 1][L_MAX + i][0];
			coordinate[L_MAX - j][L_MAX + i][1] = coordinate[L_MAX - j - 1][L_MAX + i][1] - 1;

			coordinate[L_MAX + j][L_MAX - i][0] = coordinate[L_MAX + j + 1][L_MAX - i][0];
			coordinate[L_MAX + j][L_MAX - i][1] = coordinate[L_MAX + j + 1][L_MAX - i][1] + 1;
		}

		/* diagonal */
		for (j = 0; j < i; j++) {
			coordinate[L_MAX + j + 1][L_MAX + i - j - 1][0] = coordinate[L_MAX + j][L_MAX + i - j - 1][0];
			coordinate[L_MAX + j + 1][L_MAX + i - j - 1][1] = coordinate[L_MAX + j][L_MAX + i - j - 1][1] - 1;

			coordinate[L_MAX - j - 1][L_MAX - i + j + 1][0] = coordinate[L_MAX - j][L_MAX - i + j + 1][0];
			coordinate[L_MAX - j - 1][L_MAX - i + j + 1][1] = coordinate[L_MAX - j][L_MAX - i + j + 1][1] + 1;
		}	
	}


	/* set outer side as L_MAX + 1 */
	for (i = 0; i < L_MAX * 2 + 1; i++) {
		for (j = 0; j < L_MAX * 2 + 1; j++) {
			if (coordinate[i][j][0] == 0 && coordinate[i][j][1] == 0) {
				if (i != L_MAX && j != L_MAX) {
					coordinate[i][j][0] = L_MAX + 1;
					coordinate[i][j][1] = L_MAX + 1;
				}
			}
		}
	}
}

/* Initialize pheromone distribution */
void pheromoneInitialization () {
	int i, j;

	for (i = 0; i < L_MAX * 2 + 1; i++) {
		for (j = 0; j < L_MAX * 2 + 1; j++) {
			if (coordinate[i][j][0] <= L_MAX) {
				pheromone[i][j][0] = 0;
				pheromone[i][j][1] = 0;

				tempPheromone[i][j][0] = 0;
				tempPheromone[i][j][1] = 0;
			} 
			else {
				pheromone[i][j][0] = -1000;
				pheromone[i][j][0] = -1000;

				tempPheromone[i][j][0] = -1000;
				tempPheromone[i][j][1] = -1000;
			}
		}
	}
}


/* Initialize ants structs */
void antInitialization () {
	int a_n;
	int r = rand () % 2;
	for (a_n = 0; a_n < ANT_NUM; a_n++) {
		antList[a_n].x = L_MAX;
		antList[a_n].y = L_MAX;
		antList[a_n].direction = rand() % 6;
		antList[a_n].mode = 1;
		antList[a_n].vitality = N;
		antList[a_n].hasFood = 0;
		antList[a_n].hasComeBack = 0;

		r = rand () % 2;
		if (r == 0) {
			antList[a_n].threshold[0] = THRESHOLD_ONE_SENSITIVE;
			antList[a_n].threshold[1] = THRESHOLD_TWO_SENSITIVE;
		}
		else {
			antList[a_n].threshold[0] = THRESHOLD_ONE_INSENSITIVE;
			antList[a_n].threshold[1] = THRESHOLD_TWO_INSENSITIVE;
		}
		totalAnt++;
	}
}

/* Puts food at somewhere */
void foodInitialization () {
	int f;
	int randomX, randomY;

	if (DOES_SET_FOOD == 1) {
		printf ("FOOD PUTTING MODE, JUST COPY AND PASTE\n");

		for (f = 0; f < FOOD_NUM * 7; f++) {
			scanf("%d%d", &randomX, &randomY);
			foodPos[f][0] = randomX;
			foodPos[f][1] = randomY;
		}

		return;
	}

	for (f = 0; f < FOOD_NUM; f++) {
		randomX = (rand () % L_MAX) + (L_MAX / 2);
		randomY = (rand () % L_MAX) + (L_MAX / 2);

		while (coordinate[randomX][randomY][0] > L_MAX) {
			randomX = rand() % (L_MAX * 2 + 1);
			randomY = rand() % (L_MAX * 2 + 1);
		}

		if (SURROUNDING_MODE == 0) {
			foodPos[f][0] = randomX;
			foodPos[f][1] = randomY;
		}
		else {
			foodPos[7 * f][0] = randomX;
			foodPos[7 * f][1] = randomY;

			foodPos[7 * f + 1][0] = randomX;
			foodPos[7 * f + 1][1] = randomY - 1;

			foodPos[7 * f + 2][0] = randomX + 1;
			foodPos[7 * f + 2][1] = randomY - 1;

			foodPos[7 * f + 3][0] = randomX + 1;
			foodPos[7 * f + 3][1] = randomY;

			foodPos[7 * f + 4][0] = randomX;
			foodPos[7 * f + 4][1] = randomY + 1;

			foodPos[7 * f + 5][0] = randomX - 1;
			foodPos[7 * f + 5][1] = randomY + 1;

			foodPos[7 * f + 6][0] = randomX - 1;
			foodPos[7 * f + 6][1] = randomY;
		}

		printf("%d: %d %d\n", f, randomX, randomY);
	}
}

/* Puts obstacles */
/*
void obstacleInitialization () {
	if (OBSTACLE_MODE == 1) {
		int o;
		int randomX, randomY;

		for (o = 0; o < OBSTACLE_NUM; o++) {
			random
		}
	}
}
*/

/* Estimate how long it would take to finish */
void calculateRemaningTime () {
	int currentTime = (unsigned) time(NULL);
	double calculationSpeed;

	currentTime -= startingTime;

	calculationSpeed = currentTime / (double) procedureCount;

	remainingTime = calculationSpeed * (MAX_PROCEDURE - procedureCount) / 60.0;
}

/* draws food on a screen */
void drawAllFood () {
	int f;
	g_marker_color (3);
	g_marker_type (-3);
	g_marker_size (2);
	if (SURROUNDING_MODE == 0) {
		for (f = 0; f < FOOD_NUM; f++) {
			g_marker(coordinate[foodPos[f][0]][foodPos[f][1]][0], coordinate[foodPos[f][0]][foodPos[f][1]][1]);
		}
	}
	else {
		for (f = 0; f < FOOD_NUM * 7; f++) {
			g_marker(coordinate[foodPos[f][0]][foodPos[f][1]][0], coordinate[foodPos[f][0]][foodPos[f][1]][1]);
		}
	}
	
}

/* draws nest on a screen */
void drawNest () {
	g_marker_type (-1);
	g_marker_size (2);

	g_marker(0, 0);
}

/* Initialize a given ant */
void initializeGivenAnt (int a_n) {
	if (antList[a_n].hasComeBack == 1) {
		comeBack++;
	}
	totalAnt++;

	antList[a_n].x = L_MAX;
	antList[a_n].y = L_MAX;
	antList[a_n].direction = rand() % 6;
	antList[a_n].mode = 1;
	antList[a_n].vitality = N;
	antList[a_n].hasFood = 0;
	antList[a_n].hasComeBack = 0;

	if (rand () % 2 == 0) {
		antList[a_n].threshold[0] = THRESHOLD_ONE_SENSITIVE;
		antList[a_n].threshold[1] = THRESHOLD_TWO_SENSITIVE;
	}
	else {
		antList[a_n].threshold[0] = THRESHOLD_ONE_INSENSITIVE;
		antList[a_n].threshold[1] = THRESHOLD_TWO_INSENSITIVE;
	}
}

/* Stops this program */
void halt () {
	createAndWriteInFile();

	g_sleep(G_STOP);
	g_term();
}

/* ----------------------------- GRAPHIC ---------------------------------- */

/* Progress Bar */
void progressBar () {
	g_sel_scale (2);
	g_area_color (7);
	g_line_color (0);

	g_box (0, procedureCount + 1, 0, 10, 1, 1);
}

/* Ratio meter */
void ratioBar () {
	int a_n;

	g_sel_scale(3);

	ratio[0] = 0;
	ratio[1] = 0;
	ratio[2] = 0;

	for (a_n = 0; a_n < ANT_NUM; a_n++) {
		ratio[antList[a_n].mode - 1] ++;
	}

	g_area_color (7);
	g_box (0, ratio[0], 0, 10, 1, 1);

	g_area_color (1);
	g_box (ratio[0], ratio[0] + ratio[1], 0, 10, 1, 1);

	g_area_color (2);
	g_box (ratio[0] + ratio[1], ANT_NUM, 0, 10, 1, 1);
}

/* Homing meter */
void homingBar () {
	g_sel_scale (4);

	g_area_color (7);
	g_box (0, 1, 0, 10, 1, 1);

	g_area_color (6);
	g_box (0, comeBack / (double)totalAnt, 0, 10, 1, 1);
}

/* Normalize pheromones */
void normalizePheromone () {
	int px, py;
	double maxOne = 0;
	double maxTwo = 0;

	/* find max to normalize */
	for (px = 0; px <= L_MAX * 2; px++) {
		for (py = 0; py <= L_MAX * 2; py++) {
			maxOne = fmax (maxOne, pheromone[px][py][0]);
			maxTwo = fmax (maxTwo, pheromone[px][py][1]);

			normalizedPheromoneOne[px][py] = -0.1;
			normalizedPheromoneTwo[px][py] = -0.1;
		}
	}

	/* normalize */
	for (px = 0; px <= L_MAX * 2; px++) {
		for (py = 0; py <= L_MAX * 2; py++) {
			if (pheromone[px][py][0] >= 0) {
				normalizedPheromoneOne[px][py] = pheromone[px][py][0] / maxOne;
			}

			if (pheromone[px][py][0] >= 0) {
				normalizedPheromoneTwo[px][py] = pheromone[px][py][1] / maxTwo;
			}
		}
	}
}

/* Draws pheromone distribution */
void drawPheromoneDistribution () {
	normalizePheromone ();

	/* pheromone one */
	g_hidden(100, 100, 50, 0, 1, 500, 135, 45, 115, 5, 70, 90, (double *)normalizedPheromoneOne, 2 * L_MAX + 1, 2 * L_MAX + 1, 1, 0, 1, 1);
	/* pheromone two */
	g_hidden(100, 100, 50, 0, 1, 500, 135, 45, 215, 5, 70, 90, (double *)normalizedPheromoneTwo, 2 * L_MAX + 1, 2 * L_MAX + 1, 1, 0, 1, 1);
}

/* ------------------------- ANT_BEHAVIOR --------------------------------- */

/* Single Ant movement */
void singleAntMovement (int a_n) {
	int n;
	for (n = 0; n < N; n++) {
		singleMovement(a_n);

		g_sleep(FPS);
	}
}

/* Only single movement */
void singleMovement (int a_n) {
	plotAnt(a_n, 0);
	
	g_sel_scale (1);
	/* MoveAnt */
	if (antList[a_n].mode == 1) {
		movementOne(a_n);
		plotAnt(a_n, 7);
	}
	else if (antList[a_n].mode == 2) {
		movementTwo(a_n);
		plotAnt(a_n, 1);
	}
	else if (antList[a_n].mode == 3) {
		movementThree(a_n);
		plotAnt(a_n, 2);
	}
}

/* Multi Thread Mode */ 
void multipleMovement () {
	int a_n, n;

	g_sel_scale (1);

	for (n = 0; n < N; n++) {
		for (a_n = 0; a_n < ANT_NUM; a_n++) {
			singleMovement(a_n);
		}

		g_sleep(FPS);
	}
}

/* Consectively comes out from the nest */
void consecutiveMovement () {
	int n, a_n;
	int t = 0;
	int count = 0;
	int tempCount = 0;

	while (t < MAX_PROCEDURE) {
		g_sel_scale (1);
		if (count <= ANT_NUM) {
			for (a_n = 0; a_n < count; a_n ++) {
				singleMovement (a_n);
			}

			tempCount ++;

			if (tempCount > LUG) {
				count++;
				tempCount = 0;
			}
			g_sleep(FPS);
		}
		else {
			for (a_n = 0; a_n < ANT_NUM; a_n++) {
				if (antList[a_n].vitality < 0) {
					plotAnt(a_n, 0);
					initializeGivenAnt(a_n);
				}
				singleMovement (a_n);
			}

			g_sleep(FPS);
		}
		drawAllFood ();

		procedureCount = t;

		g_sel_scale (2);
		progressBar ();

		g_sel_scale (3);
		ratioBar ();

		g_sel_scale (4);
		homingBar ();

		pheromoneBehavior ();

		homingProb[t] = comeBack / (double)totalAnt;
		homingNum[t] = comeBack;

		if (t % N == 0) {
			calculateRemaningTime ();
			printf ("%03.1lfmin left\n", remainingTime);
		}

		t++;
	}
}

/* Mode 1 (Random movement mode) */
void movementOne (int a_n) {
	pheromone[antList[a_n].x][antList[a_n].y][0] += PHEROMONE_RELEASE_ONE;
	moveToGivenDirection(calculateDirection(1, antList[a_n].direction, a_n), a_n);

	antList[a_n].vitality --;
	/* becomes mode 2 if there is enough pheromone around */
	if (checkPheromone(a_n, 1) == 1) {
		//printf("%d becomes 2 due to pheromone\n", a_n);
		antList[a_n].mode = 2;
	}

	if (antList[a_n].vitality < 0) {
		antList[a_n].mode = 3;
	}

	if (hasDiscoveredFood(a_n) == 1) {
		antList[a_n].mode = 3;
	}
}

/* Mode 2 (Pheromone searching mode) */
void movementTwo (int a_n) {
	pheromone[antList[a_n].x][antList[a_n].y][0] += PHEROMONE_RELEASE_ONE;
	moveToGivenDirection(calculateDirection(2, antList[a_n].direction, a_n), a_n);

	antList[a_n].vitality --;
	/* becomes mode 1 if there is not enough pheromone around */
	if (checkPheromone(a_n, 1) == 0) {
		//printf("%d becomes 1 due to pheromone\n", a_n);
		antList[a_n].mode = 1;
	}

	if (antList[a_n].vitality < 0) {
		antList[a_n].mode = 3;
	}

	if (hasDiscoveredFood(a_n) == 1) {
		antList[a_n].mode = 3;
	}
}

/* Mode 3 (Homing mode) */
void movementThree (int a_n) {
	pheromone[antList[a_n].x][antList[a_n].y][1] += PHEROMONE_RELEASE_TWO;
	moveToGivenDirection(calculateDirection(3, antList[a_n].direction, a_n), a_n);


	antList[a_n].vitality --;

	if (hasGoneBack (a_n) == 1) {
		//printf("%d: Has GONE BACK\n", a_n);
		antList[a_n].hasComeBack = 1;
		antList[a_n].vitality = 0;
	}
}

/* Checks the food's existence */
int hasDiscoveredFood (int a_n) {
	int f;

	if (SURROUNDING_MODE == 0) {
		for (f = 0; f < FOOD_NUM; f++) {
			if (antList[a_n].x == foodPos[f][0] && antList[a_n].y == foodPos[f][1]) {
				//printf("%d DISCOVERED! at %d\n", a_n, f);
				antList[a_n].hasFood = 1;
				return 1;
			}
		}
	}
	else {
		for (f = 0; f < FOOD_NUM * 7; f++) {
			if (antList[a_n].x == foodPos[f][0] && antList[a_n].y == foodPos[f][1]) {
				//printf("%d DISCOVERED! at %d\n", a_n, f);
				antList[a_n].hasFood = 1;
				return 1;
			}
		}
	}

	return 0;
}

/* Calculate Direction */
int calculateDirection (int mode, int dir, int a_n) {
	int randomDir, i, s;
	int maxDir;
	int surroundingPheromone[6];
	double normalizedSurrounding[6];
	int monteCalro;
	int whichPheromone;

	/* for mode one */
	if (mode == 1) {
		randomDir = rand() % 3;

		/* Up */
		if (dir == 0) {
			if (randomDir == 0) {
				return 5;
			}
			else if (randomDir == 1) {
				return 0;
			}
			else {
				return 1;
			}
		}
		/* Right up */
		else if (dir == 1) {
			if (randomDir == 0) {
				return 0;
			}
			else if (randomDir == 1) {
				return 1;
			}
			else {
				return 2;
			}
		}
		/* Right down */
		else if (dir == 2) {
			if (randomDir == 0) {
				return 1;
			}
			else if (randomDir == 1) {
				return 2;
			}
			else {
				return 3;
			}
		}
		/* Down */
		else if (dir == 3) {
			if (randomDir == 0) {
				return 2;
			}
			else if (randomDir == 1) {
				return 3;
			}
			else {
				return 4;
			}
		}
		/* Left down */
		else if (dir == 4) {
			if (randomDir == 0) {
				return 3;
			}
			else if (randomDir == 1) {
				return 4;
			}
			else {
				return 5;
			}
		}
		else if (dir == 5) {
			if (randomDir == 0) {
				return 4;
			}
			else if (randomDir == 1) {
				return 5;
			}
			else {
				return 0;
			}
		}
	}

	/* for mode two and three */
	else {
		/* Which pheromone to think */
		if (mode == 2) {
			whichPheromone = 1;
		}
		else {
			whichPheromone = 0;
		}

		/* Initialize */
		for (i = 0; i < 6; i++) {
			surroundingPheromone[i] = 1;
		}

		/* Calculating (Surrounding) */
		/* Up */
		if (antList[a_n].x - 1 >= 0) {
			if (coordinate[antList[a_n].x - 1][antList[a_n].y][0] <= L_MAX) {
				surroundingPheromone[0] += (int)INCREMENT * pheromone[antList[a_n].x - 1][antList[a_n].y][whichPheromone];
			}
		}
		/* Right up */
		if (antList[a_n].x - 1 >= 0 && antList[a_n].y + 1 <= L_MAX * 2) {
			if (coordinate[antList[a_n].x - 1][antList[a_n].y + 1][0] <= L_MAX) {
				surroundingPheromone[1] += (int)INCREMENT * pheromone[antList[a_n].x - 1][antList[a_n].y + 1][whichPheromone];		
			}	
		}
		/* Right down */
		if (antList[a_n].y + 1 <= L_MAX * 2) {
			if (coordinate[antList[a_n].x][antList[a_n].y + 1][0] <= L_MAX) {
				surroundingPheromone[2] += (int)INCREMENT * pheromone[antList[a_n].x][antList[a_n].y + 1][whichPheromone];	
			}
		}
		/* Down */
		if (antList[a_n].x + 1 <= L_MAX * 2) {
			if (coordinate[antList[a_n].x + 1][antList[a_n].y][0] <= L_MAX) {
				surroundingPheromone[3] += (int)INCREMENT * pheromone[antList[a_n].x + 1][antList[a_n].y][whichPheromone];	
			}
		}
		/* Left down */
		if (antList[a_n].x + 1 <= L_MAX * 2 && antList[a_n].y - 1 >= 0) {
			if (coordinate[antList[a_n].x + 1][antList[a_n].y - 1][0] <= L_MAX) {
				surroundingPheromone[4] += (int)INCREMENT * pheromone[antList[a_n].x + 1][antList[a_n].y - 1][whichPheromone];				
			}
		}
		/* Left up */
		if (antList[a_n].y - 1 >= 0) {
			if (coordinate[antList[a_n].x][antList[a_n].y - 1][0] <= L_MAX) {
				surroundingPheromone[5] += (int)INCREMENT * pheromone[antList[a_n].x][antList[a_n].y - 1][whichPheromone];
			}
		}

		maxDir = 0;
		for (s = 0; s < 6; s++) {
			if (surroundingPheromone[s] < 0) {
				surroundingPheromone[s] = RAND_MAX;
			}

			if (surroundingPheromone[s] > surroundingPheromone[maxDir]) {
				maxDir = s;
			}
		}


		return calculateProb(surroundingPheromone, dir);
		/* Calculate Direction */
		/* Up */
		if (dir == 0) {
			monteCalro = surroundingPheromone[0] + surroundingPheromone[1] + surroundingPheromone[5];
			randomDir = (rand() % monteCalro);

			/*
			if (surroundingPheromone[0] > surroundingPheromone[1]) {
				if (surroundingPheromone[5] > surroundingPheromone[0]) {
					return 5;
				}
				else {
					return 0;
				}
			}
			else {
				if (surroundingPheromone[1] > surroundingPheromone[5]) {
					return 1;
				}
				else {
					return 5;
				}
			}
			*/

			if (randomDir < surroundingPheromone[0]) {
				return 0;
			}
			else if (randomDir < surroundingPheromone[0] + surroundingPheromone[1]) {
				return 1;
			}
			else {
				return 5;
			}
		}
		/* Right up */
		else if (dir == 1) {
			monteCalro = surroundingPheromone[1] + surroundingPheromone[2] + surroundingPheromone[0];
			
			randomDir = (rand() % monteCalro);

			/*
			if (surroundingPheromone[2] > surroundingPheromone[0]) {
				if (surroundingPheromone[1] > surroundingPheromone[2]) {
					return 1;
				}
				else {
					return 2;
				}
			}
			else {
				if (surroundingPheromone[0] > surroundingPheromone[1]) {
					return 0;
				}
				else {
					return 1;
				}
			}
			*/

			if (randomDir < surroundingPheromone[1]) {
				return 1;
			}
			else if (randomDir < surroundingPheromone[1] + surroundingPheromone[2]) {
				return 2;
			}
			else {
				return 0;
			}
		}
		/* Right down */
		else if (dir == 2) {
			monteCalro = surroundingPheromone[2] + surroundingPheromone[3] + surroundingPheromone[1];
			
			randomDir = (rand() % monteCalro);

			/*
			if (surroundingPheromone[2] > surroundingPheromone[3]) {
				if (surroundingPheromone[1] > surroundingPheromone[2]) {
					return 1;
				}
				else {
					return 2;
				}
			}
			else {
				if (surroundingPheromone[3] > surroundingPheromone[1]) {
					return 3;
				}
				else {
					return 1;
				}
			}
			*/

			if (randomDir < surroundingPheromone[2]) {
				return 2;
			}
			else if (randomDir < surroundingPheromone[2] + surroundingPheromone[3]) {
				return 3;
			}
			else {
				return 1;
			}
		}
		/* Down */
		else if (dir == 3) {
			monteCalro = surroundingPheromone[3] + surroundingPheromone[4] + surroundingPheromone[2];

			randomDir = (rand() % monteCalro);

			/*
			if (surroundingPheromone[3] > surroundingPheromone[4]) {
				if (surroundingPheromone[2] > surroundingPheromone[3]) {
					return 2;
				}
				else {
					return 3;
				}
			}
			else {
				if (surroundingPheromone[4] > surroundingPheromone[2]) {
					return 4;
				}
				else {
					return 2;
				}
			}
			*/

			if (randomDir < surroundingPheromone[3]) {
				return 3;
			}
			else if (randomDir < surroundingPheromone[3] + surroundingPheromone[4]) {
				return 4;
			}
			else {
				return 2;
			}
		}
		/* Left down */
		else if (dir == 4) {
			monteCalro = surroundingPheromone[4] + surroundingPheromone[5] + surroundingPheromone[3];

			randomDir = (rand() % monteCalro);

			/*
			if (surroundingPheromone[4] > surroundingPheromone[5]) {
				if (surroundingPheromone[3] > surroundingPheromone[4]) {
					return 3;
				}
				else {
					return 4;
				}
			}
			else {
				if (surroundingPheromone[5] > surroundingPheromone[3]) {
					return 5;
				}
				else {
					return 3;
				}
			}
			*/

			if (randomDir < surroundingPheromone[4]) {
				return 4;
			}
			else if (randomDir < surroundingPheromone[4] + surroundingPheromone[5]) {
				return 5;
			}
			else {
				return 3;
			}
		}
		/* Left up */
		else if (dir == 5) {
			monteCalro = surroundingPheromone[5] + surroundingPheromone[0] + surroundingPheromone[4];

			randomDir = (rand() % monteCalro);

			/*
			if (surroundingPheromone[5] > surroundingPheromone[0]) {
				if (surroundingPheromone[4] > surroundingPheromone[5]) {
					return 4;
				}
				else {
					return 5;
				}
			}
			else {
				if (surroundingPheromone[0] > surroundingPheromone[4]) {
					return 0;
				}
				else {
					return 4;
				}
			}
			*/

			if (randomDir < surroundingPheromone[5]) {
				return 5;
			}
			else if (randomDir < surroundingPheromone[5] + surroundingPheromone[0]) {
				return 0;
			}
			else {
				return 4;
			}
		}
	}

	return -1;
}

/* Calculate probability of the direction */
int calculateProb (int tempPheromone[6], int dir) {
	int d, random;
	int monteCalro;
	int inc = 16;
	double max;
	double normalizedSurrounding[6];
	int surrounding[6];

	for (d = 0; d < 6; d++) {
		if (tempPheromone[d] < 0) {
			tempPheromone[d] = RAND_MAX;
		}

		max = fmax(max, tempPheromone[d]);
	}

	for (d = 0; d < 6; d++) {
		normalizedSurrounding[d] = tempPheromone[d] / max;
		surrounding[d] = (int) exp (inc * normalizedSurrounding[d]);
	}

	if (dir == 0) {
		monteCalro = surrounding[5] + surrounding[0] + surrounding[1];
		random = rand () % monteCalro;
		
		if (random < surrounding[5]) {
			return 5;
		}
		else if (random < surrounding[5] + surrounding[0]) {
			return 0;
		}
		else {
			return 1;
		}
	}
	else if (dir == 1) {
		monteCalro = surrounding[0] + surrounding[1] + surrounding[2];
		random = rand () % monteCalro;
		
		if (random < surrounding[0]) {
			return 0;
		}
		else if (random < surrounding[0] + surrounding[1]) {
			return 1;
		}
		else {
			return 2;
		}
	}
	else if (dir == 2) {
		monteCalro = surrounding[1] + surrounding[2] + surrounding[3];
		random = rand () % monteCalro;
		
		if (random < surrounding[1]) {
			return 1;
		}
		else if (random < surrounding[1] + surrounding[2]) {
			return 2;
		}
		else {
			return 3;
		}
	}
	else if (dir == 3) {
		monteCalro = surrounding[2] + surrounding[3] + surrounding[4];
		random = rand () % monteCalro;
		
		if (random < surrounding[2]) {
			return 2;
		}
		else if (random < surrounding[2] + surrounding[3]) {
			return 3;
		}
		else {
			return 4;
		}
	}
	else if (dir == 4) {
		monteCalro = surrounding[3] + surrounding[4] + surrounding[5];
		random = rand () % monteCalro;
		
		if (random < surrounding[3]) {
			return 3;
		}
		else if (random < surrounding[3] + surrounding[4]) {
			return 4;
		}
		else {
			return 5;
		}
	}
	else if (dir == 5) {
		monteCalro = surrounding[4] + surrounding[5] + surrounding[0];
		random = rand () % monteCalro;
		
		if (random < surrounding[4]) {
			return 4;
		}
		else if (random < surrounding[4] + surrounding[5]) {
			return 5;
		}
		else {
			return 0;
		}
	}

	return 0;
}

/* Single movement */
void moveToGivenDirection (int dir, int a_n) {
	/* Up */
	if (dir == 0) {
		if (antList[a_n].x - 1 >= 0) {
			if (coordinate[antList[a_n].x - 1][antList[a_n].y][0] <= L_MAX) {
				antList[a_n].x --;			
			}
		}
		antList[a_n].direction = 0;	
	}
	/* Right up */
	else if (dir == 1) {
		if (antList[a_n].x - 1 >= 0 && antList[a_n].y + 1 <= L_MAX * 2) {
			if (coordinate[antList[a_n].x - 1][antList[a_n].y + 1][0] <= L_MAX) {
				antList[a_n].x --;
				antList[a_n].y ++;				
			}	
		}
		antList[a_n].direction = 1;
 	}
 	/* Right down */
	else if (dir == 2) {
		if (antList[a_n].y + 1 <= L_MAX * 2) {
			if (coordinate[antList[a_n].x][antList[a_n].y + 1][0] <= L_MAX) {
				antList[a_n].y ++;
			}
		}
		antList[a_n].direction = 2;
	}
	/* Down */
	else if (dir == 3) {
		if (antList[a_n].x + 1 <= L_MAX * 2) {
			if (coordinate[antList[a_n].x + 1][antList[a_n].y][0] <= L_MAX) {
				antList[a_n].x ++;
			}
		}
		antList[a_n].direction = 3;
	}
	/* Left down */
	else if (dir == 4) {
		if (antList[a_n].x + 1 <= L_MAX * 2 && antList[a_n].y - 1 >= 0) {
			if (coordinate[antList[a_n].x + 1][antList[a_n].y - 1][0] <= L_MAX) {
				antList[a_n].x ++;
				antList[a_n].y --;				
			}
		}
		antList[a_n].direction = 4;
	}
	/* Left up */
	else if (dir == 5) {
		if (antList[a_n].y - 1 >= 0) {
			if (coordinate[antList[a_n].x][antList[a_n].y - 1][0] <= L_MAX) {
				antList[a_n].y --;
			}
		}
		antList[a_n].direction = 5;
	}
}
/* Plot a_n'th ant with given color */
void plotAnt (int a_n, int color) {
	g_marker_size(1);
	g_marker_type(0);
	g_marker_color(color);
	g_marker(coordinate[antList[a_n].x][antList[a_n].y][0], coordinate[antList[a_n].x][antList[a_n].y][1]);
}

/* Checks whether an ant has gone back to the colony */
int hasGoneBack (int a_n) {
	if (antList[a_n].x == L_MAX && antList[a_n].y == L_MAX) {
		return 1;
	}
	return 0;
} 

/* Checks whether there is site where pheromone is over threshold */
int checkPheromone (int a_n, int pheromone_num) {
	/* Up */
	if (antList[a_n].y - 1 >= 0) {
		if (pheromone[antList[a_n].x][antList[a_n].y - 1][pheromone_num] >= antList[a_n].threshold[pheromone_num]) {
			return 1;
		}
	}

	/* Right up */
	if (antList[a_n].x + 1 <= L_MAX * 2 && antList[a_n].y - 1 >= 0) {
		if (pheromone[antList[a_n].x + 1][antList[a_n].y - 1][pheromone_num] >= antList[a_n].threshold[pheromone_num]) {
			return 1;			
		}
	}

	/* Right down */
	if (antList[a_n].x + 1 <= L_MAX * 2) {
		if (pheromone[antList[a_n].x + 1][antList[a_n].y][pheromone_num] >= antList[a_n].threshold[pheromone_num]) {
			return 1;
		}
	}

	/* Down */
	if (antList[a_n].y + 1 <= L_MAX * 2) {
		if (pheromone[antList[a_n].x][antList[a_n].y + 1][pheromone_num] >= antList[a_n].threshold[pheromone_num]) {
			return 1;
		}
	}

	/* Left down */
	if (antList[a_n].x - 1 >= 0 && antList[a_n].y + 1 <= L_MAX * 2) {
		if (pheromone[antList[a_n].x - 1][antList[a_n].y + 1][pheromone_num] >= antList[a_n].threshold[pheromone_num]) {
			return 1;				
		}	
	}

	/* Left up */
	if (antList[a_n].x - 1 >= 0) {
		if (pheromone[antList[a_n].x - 1][antList[a_n].y][pheromone_num] >= antList[a_n].threshold[pheromone_num]) {
			return 1;		
		}
	}

	return 0;
}

/* ------------------------- PHEROMONE BEHAVIOR -------------------------- */

/* Pheromone chage according to time */
void pheromoneBehavior () {
	pheromoneDiffusion ();
	pheormoneEvaporation ();
}

/* Pheromone diffusion */
void pheromoneDiffusion () {
	int px, py;
	double tempDiffOne, tempDiffTwo;

	/* Store pheromone that will diffuse */
	for (px = 0; px <= L_MAX * 2; px++) {
		for (py = 0; py <= L_MAX * 2; py++) {
			if (pheromone[px][py][0] > 0) {
				tempDiffOne = pheromone[px][py][0] * DIFFUSION_COEF_ONE;
				tempDiffTwo = pheromone[px][py][1] * DIFFUSION_COEF_TWO;

				/* Up */
				if (px - 1 >= 0) {
					if (tempPheromone[px - 1][py][0] >= 0) {
						tempPheromone[px - 1][py][0] += tempDiffOne;
						tempPheromone[px - 1][py][1] += tempDiffTwo;
					}
				}
				/* Right up */
				if (px - 1 >= 0 && py + 1 <= L_MAX * 2) {
					if (tempPheromone[px - 1][py + 1][0] >= 0) {
						tempPheromone[px - 1][py + 1][0] += tempDiffOne;
						tempPheromone[px - 1][py + 1][1] += tempDiffTwo;
					}
				}
				/* Right down */
				if (py + 1 <= L_MAX * 2) {
					if (tempPheromone[px][py + 1][0] >= 0) {
						tempPheromone[px][py + 1][0] += tempDiffOne;
						tempPheromone[px][py + 1][1] += tempDiffTwo;
					}
				}
				/* Left down */
				if (px + 1 <= L_MAX * 2) {
					if (tempPheromone[px + 1][py][0] >= 0) {
						tempPheromone[px + 1][py][0] += tempDiffOne;
						tempPheromone[px + 1][py][1] += tempDiffTwo;
					}
				}
				/* Left up */
				if (px + 1 <= L_MAX * 2 && py - 1 >= 0) {
					if (tempPheromone[px + 1][py - 1][0] >= 0) {
						tempPheromone[px + 1][py - 1][0] += tempDiffOne;
						tempPheromone[px + 1][py - 1][1] += tempDiffTwo;
					}
				}
				/* Down */
				if (py - 1 >= 0) {
					if (tempPheromone[px][py - 1][0] >= 0) {
						tempPheromone[px][py - 1][0] += tempDiffOne;
						tempPheromone[px][py - 1][1] += tempDiffTwo;
					}
				}
			}
		}
	}
	
	/* Diffusion */
	for (px = 0; px <= L_MAX * 2; px++) {
		for (py = 0; py <= L_MAX * 2; py++) {
			if (tempPheromone[px][py][0] > 0) {
				pheromone[px][py][0] -= 6 * tempDiffOne;
				pheromone[px][py][1] -= 6 * tempDiffTwo;

				pheromone[px][py][0] += tempPheromone[px][py][0];
				pheromone[px][py][1] += tempPheromone[px][py][1];
			}
		}
	}
}

/* Pheromone evaporation */
void pheormoneEvaporation () {
	int px, py;

	for (px = 0; px <= L_MAX * 2; px++) {
		for (py = 0; py <= L_MAX * 2; py++) {
			if (pheromone[px][py][0] > 0) {
				pheromone[px][py][0] *= EVAPORATE_COEF_ONE;
				pheromone[px][py][1] *= EVAPORATE_COEF_TWO;
			}
		}
	}
}

/* ---------------------------- FILE OUTPUT ------------------------------ */

/* creates file with current time */
void createAndWriteInFile () {
	char fileName[20];
	FILE *fp;
	int t;

	sprintf (fileName, "%d.csv", seed);

	if ((fp = fopen (fileName, "w")) == NULL) {
		printf ("FAILED TO CREATE FILE");
	}

	fprintf (fp, "L_MAX,ANT_NUM,N,FOOD_NUM,SURROUNDING_MODE,MAX_PROCEDURE,LUG,THRESHOLD_TWO_SENSITIVE,THRESHOLD_TWO_INSENSITIVE,PHEROMONE_RELEASE_ONE,PHEROMONE_RELEASE_TWO,INCREMENT,DIFFUSION_COEF_ONE,EVAPORATE_COEF_ONE,DIFFUSION_COEF_TWO,EVAPORATE_COEF_TWO\n");
	fprintf (fp, "%d,%d,%d,%d,%d,%d,%lf,%lf,%lf,%lf,%lf,%d,%lf,%lf,%lf,%lf\n", L_MAX, ANT_NUM, N,FOOD_NUM, SURROUNDING_MODE, MAX_PROCEDURE, LUG, THRESHOLD_TWO_SENSITIVE, THRESHOLD_TWO_INSENSITIVE, PHEROMONE_RELEASE_ONE, PHEROMONE_RELEASE_TWO, INCREMENT, DIFFUSION_COEF_ONE, EVAPORATE_COEF_ONE, DIFFUSION_COEF_TWO,  EVAPORATE_COEF_TWO);

	for (t = 0; t < MAX_PROCEDURE; t++) {
		fprintf (fp, "%d,%lf, %d", t, homingProb[t], homingNum[t]);

		if (t < 7 * FOOD_NUM) {
			fprintf (fp, ",%d,%d", foodPos[t][0], foodPos[t][1]);
		}

		fprintf (fp, "\n");
	}

	fclose (fp);
}


/* ----------------------------- TEST ------------------------------------ */
/* Checking if honeycomb is properly generated */
void graphicTest () {
	int i, j;

	g_def_scale(1, -L_MAX - 1, L_MAX + 1, - L_MAX - 1, L_MAX + 1, 0, 0, 100.0, 100.0);
	g_marker_type(1);
	g_marker_color(0);

	g_sel_scale(1);

	for (i = 0; i < L_MAX * 2 + 1; i++) {
		for (j = 0; j < L_MAX * 2 + 1; j++) {
			g_marker (coordinate[i][j][0], coordinate[i][j][1]);
		}
	}
}

void structAntTest (int a_n) {
	printf("%d, %d", antList[a_n].x, antList[a_n].y);
}
