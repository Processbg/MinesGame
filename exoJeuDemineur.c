#include "graphics.h"

#define case_de_carre 20

int plateau_diminateur[20][20];

int mode_decouvrir[20][20];

int mine_conte;

int trouver_voisins(int x, int y) 
{
	int conter = 0;
	
	if (x-1 >= 0 && y-1 >= 0)
		if (plateau_diminateur[x-1][y-1] == -1)
		  conter++;

	if (y-1 >= 0)
		if (plateau_diminateur[x][y-1] == -1)
		  conter++;

	if (x+1 < case_de_carre && y-1 >= 0)
		if (plateau_diminateur[x+1][y-1] == -1)
		  conter++;
		  
	if (x-1 >= 0)
		if (plateau_diminateur[x-1][y] == -1)
		  conter++;

	if (x+1 < case_de_carre)
		if (plateau_diminateur[x+1][y] == -1)
			conter++;
			
	if (y+1 < case_de_carre && x-1 >= 0)
		if (plateau_diminateur[x-1][y+1] == -1)
			conter++;

	if (y+1 < case_de_carre)
		if (plateau_diminateur[x][y+1] == -1)
			conter++;

	if (y+1 < case_de_carre && x + 1 < case_de_carre)
		if (plateau_diminateur[x+1][y+1] == -1)
			conter++;

	return conter;
}

void inisialise_plateau()
{
	int i, j, m;
	
	mine_conte = 0;
	
	for(i=0; i <case_de_carre; i++)
		for(j=0; j<case_de_carre; j++)
		{
			m = alea_int(10);
			
			if (m ==0) 
			
			{
			   plateau_diminateur[i][j] = -1;
			   mine_conte++;
		    }
			else
			   plateau_diminateur[i][j] = 0;
			   
			mode_decouvrir[i][j] = 0;
		}
		
	for(i=0; i<case_de_carre; i++)
		for(j=0; j<case_de_carre; j++) 
		{
			
			if (plateau_diminateur[i][j] == -1)
			continue;
			
		    int conter = trouver_voisins(i, j);
		    
		    if (conter > 0)
				plateau_diminateur[i][j] = conter;
		}		
}


POINT calc_bg(int i , int j)
{
	POINT bg;
	
	bg.x = i * case_de_carre;
	bg.y = j * case_de_carre;
	
	return bg;
}

POINT calc_hd(int i, int j)
{
	POINT hd;
	
	hd.x = (i * case_de_carre) + case_de_carre;
	hd.y =  (j * case_de_carre) + case_de_carre;
	
	return hd;
}

POINT calc_centre(int i , int j)
{
	POINT centre;
	
	centre.x = (i * case_de_carre) + (case_de_carre/2);
	centre.y =  (j * case_de_carre) + (case_de_carre/2);
	
	return centre;
}

void affiche_carre(int i, int j, int visible)
{
	POINT bg = calc_bg(i,j), bd, hg, hd = calc_hd(i,j), centre = calc_centre(i,j);
	
	bd.x = bg.x + case_de_carre; hg.x = hd.x - case_de_carre;
	bd.y = bg.y; hg.y = hd.y;
	
	POINT tx;
	tx.x = (hd.x + hg.x) / 2 - 3;
	tx.y = hd.y;
	
	if(visible == 0)
	{
		//razkrito kradratche
		
		draw_fill_rectangle(bg, hd, gris);
		draw_rectangle(bg, hd, noir);
		
		return;
	}
	
	if(plateau_diminateur[i][j] == -1)
	{	
		//mina
		
		draw_line(bg, hd, rouge);
		draw_line(bd, hg, rouge);
	}
	
	if(plateau_diminateur[i][j] == 0)
	{
		//prazno kvadradche

		draw_fill_rectangle(bg, hd, blanc);
	}
	
	if(plateau_diminateur[i][j] > 0)
	{
		//kvadradche s chislo
		
		draw_fill_circle(centre, case_de_carre/2, vert);
		
		char buf[2];
		buf[0] = (char)(((int) '0') + plateau_diminateur[i][j]);
		buf[1]  = 0;
		
		aff_pol(buf, 12, tx, rouge);
	}
}

void affiche_plateau()
{
	int i,j;
	
	for(i=0; i<case_de_carre; i++)
		for(j=0; j<case_de_carre; j++)
		{
			affiche_carre(i, j, 1);
		}
}

void affiche_plateau_decouvert()
{
	int i,j;
	
	for(i=0; i<case_de_carre; i++)
		for(j=0; j<case_de_carre; j++)
			{	
				affiche_carre(i, j, mode_decouvrir[i][j]);	
			}
}

POINT calc_case_clic(POINT p)
{
	POINT centre;
	
	centre.x = p.x/case_de_carre;
	centre.y = p.y/case_de_carre;
	
	return centre;
}

void cascade(int x, int y) {

	if (mode_decouvrir[x][y] == 1)
		return;
	
	if (plateau_diminateur[x][y]<-1)
		return;
	
    mode_decouvrir[x][y] = 1;
    
	if (plateau_diminateur[x][y] > 0)
		return;
	 
	if (x-1 >= 0 && y-1 >= 0)
		cascade(x-1, y-1);

	if (y-1 >= 0)
		cascade(x, y-1);

	if (x+1 < case_de_carre && y-1 >= 0)
		cascade(x+1, y-1);
		  
	if (x-1 >= 0)
		cascade(x-1, y);

	if (x+1 < case_de_carre)
	  cascade(x+1,y);
			
	if (y+1 < case_de_carre && x-1 >= 0)
		cascade(x-1, y+1);

	if (y+1 < case_de_carre)
		cascade(x, y+1);

	if (y+1 < case_de_carre && x+1 < case_de_carre)
		cascade(x+1, y+1);
	 
}

BOOL modifie_plateau_diminateur(POINT p)
{
	POINT case_clic = calc_case_clic(p);

	if (mode_decouvrir[case_clic.x][case_clic.y] == 1)
		return FALSE;
	
	int etat_actuel = plateau_diminateur[case_clic.x][case_clic.y];
	
	if (etat_actuel < 0) 
	{
		int i, j;
		
		for(i=0; i<case_de_carre; i++)
			for(j=0; j<case_de_carre; j++)
			{
				  mode_decouvrir[i][j] = 1;
			}
		return TRUE;
	}
	
	
	if (etat_actuel > 0) 
	{
		mode_decouvrir[case_clic.x][case_clic.y] = 1;
		return FALSE;
	}
	
	cascade(case_clic.x,case_clic.y);

	return FALSE;
}

BOOL verifions_mines()
{
	int i,j;
	
	int conte_cache = 0;
	
	for(i=0; i<case_de_carre; i++)
		for(j=0; j<case_de_carre; j++)
		{
			if (mode_decouvrir[i][j] == 0)
			conte_cache++;
		}
		
	if (mine_conte == conte_cache)
		return TRUE;
		
	return FALSE;
}

void jeu()
{
	BOOL  tout_memes = FALSE;
	POINT p;
	
	while(!tout_memes)
	{
		p = wait_clic();
		tout_memes = modifie_plateau_diminateur(p);
		
		if (verifions_mines() == TRUE) 
		{
			affiche_plateau();
			tout_memes = TRUE;
		} 
		else 
		{
			affiche_plateau_decouvert();
		}
	}
}

int main()
{	
	int L = 400, H = 400;
	
	init_graphics(L,H);
	
	inisialise_plateau();
	affiche_plateau_decouvert();
	jeu();
	
	wait_escape();
	exit(0);
}
