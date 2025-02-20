static Clr **statusscheme;

int width_pwrl_status(Bar *bar, BarArg *a)
{
	return widthpowerlinestatus(stext);
}

int draw_pwrl_status(Bar *bar, BarArg *a)
{
	return drawpowerlinestatus(a->x + a->w, stext, a);
}

int click_pwrl_status(Bar *bar, Arg *arg, BarArg *a)
{
	dwmblockssig = 0;
	char status[512];
	int x = a->w;
	int i, n = strlen(stext);
	int plw = drw->fonts->h / 2 + 1;
	char *bs, bp = '|';
	strcpy(status, stext);

	for (i = n, bs = &status[n - 1]; i >= 0; i--, bs--)
	{
		if (*bs == '<' || *bs == '/' || *bs == '\\' || *bs == '>' || *bs == '|')
		{ /* block start */

			if (bp != '|')
				x -= plw;
			x -= TEXTW(bs + 3);

			if (x <= a->x) {
				dwmblockssig = ((int)*(bs + 1)) - 1;
				break;
			}

			bp = *bs;
			*bs = 0;
		}
	}

	return ClkStatusText;
}

int widthpowerlinestatus(char *stext)
{
	char status[512];
	int w = 0, i, n = strlen(stext);
	int plw = drw->fonts->h / 2 + 1;
	char *bs, bp = '|';
	strcpy(status, stext);

	for (i = n, bs = &status[n - 1]; i >= 0; i--, bs--)
	{
		if (*bs == '<' || *bs == '/' || *bs == '\\' || *bs == '>' || *bs == '|')
		{ /* block start */
			if (bp != '|')
				w += plw;
			w += TEXTW(bs + 3);
			bp = *bs;
			*bs = 0;
		}
	}
	if (bp != '|')
		w += plw * 2;

	return w;
}

int drawpowerlinestatus(int xpos, char *stext, BarArg *barg)
{
	char status[512];
	int i, n = strlen(stext), cn = 0;
	int x = xpos, w = 0;
	int plw = drw->fonts->h / 2 + 1;
	char *bs, bp = '|';
	Clr *prevscheme = statusscheme[0], *nxtscheme;
	strcpy(status, stext);

	for (i = n, bs = &status[n - 1]; i >= 0; i--, bs--)
	{
		if (*bs == '<' || *bs == '/' || *bs == '\\' || *bs == '>' || *bs == '|')
		{ /* block start */
			cn = ((int)*(bs + 2)) - 1;

			if (cn < LENGTH(statuscolors))
			{
				drw_settrans(drw, prevscheme, (nxtscheme = statusscheme[cn]));
			}
			else
			{
				drw_settrans(drw, prevscheme, (nxtscheme = statusscheme[0]));
			}

			if (bp != '|')
			{
				drw_arrow(drw, x - plw, barg->y, plw, barg->h, bp == '\\' || bp == '>' ? 1 : 0, bp == '<' ? 0 : 1);
				x -= plw;
			}

			drw_setscheme(drw, nxtscheme);
			w = TEXTW(bs + 3);
			drw_text(drw, x - w, barg->y, w, barg->h, lrpad / 2, bs + 3, 0, False);
			x -= w;

			bp = *bs;
			*bs = 0;
			prevscheme = nxtscheme;
		}
	}
	if (bp != '|')
	{
		drw_settrans(drw, prevscheme, scheme[SchemeNorm]);
		drw_arrow(drw, x - plw, barg->y, plw, barg->h, bp == '\\' || bp == '>' ? 1 : 0, bp == '<' ? 0 : 1);
		drw_rect(drw, x - 2 * plw, barg->y, plw, barg->h, 1, 1);
		x -= plw * 2;
	}

	return xpos - x;
}
