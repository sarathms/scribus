//
//  Little cms - profiler construction set
//  Copyright (C) 1998-2001 Marti Maria
//
// THIS SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
// EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
// WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
//
// IN NO EVENT SHALL MARTI MARIA BE LIABLE FOR ANY SPECIAL, INCIDENTAL,
// INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
// OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
// WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF
// LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
// OF THIS SOFTWARE.
//
// This file is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// As a special exception to the GNU General Public License, if you
// distribute this file as part of a program that contains a
// configuration script generated by Autoconf, you may include it under
// the same distribution terms that you use for the rest of that program.
//
// Version 1.09a


#include "lcmsprf.h"


LPMATN      cdecl MATNalloc(int Rows, int Cols);
void        cdecl MATNfree (LPMATN mat);
LPMATN      cdecl MATNmult(LPMATN a1, LPMATN a2);
double      cdecl MATNcross(LPMATN a);
void        cdecl MATNscalar (LPMATN a, double scl, LPMATN b);
LPMATN      cdecl MATNtranspose (LPMATN a);
BOOL        cdecl MATNsolve(LPMATN a, LPMATN b);


// ------------------------------------------------------------ Implementation

// Free matrix

void MATNfree(LPMATN mat)
{
    int i;

    if (mat == NULL) return;

    for (i = 0; i < mat->Rows; i++)
    {
        if (mat -> Values[i] != NULL)
            free (mat->Values[i]);
    }

    free(mat->Values);
    free(mat);
}


// Allocate (and Zero) a new matrix

LPMATN MATNalloc(int Rows, int Cols)
{
    int i;

    LPMATN mat = (LPMATN) malloc (sizeof (MATN));
    if (mat == NULL) return mat;

    ZeroMemory(mat, sizeof(MATN));

    mat->Rows = Rows;
    mat->Cols = Cols;
    mat->Values = (double**) malloc(Rows * sizeof (double*));

    if (mat->Values == NULL) {
        free(mat);
        return NULL;
    }

    ZeroMemory(mat -> Values, Rows * sizeof (double*));

    for (i = 0; i < Rows; i++)
    {
        mat-> Values [i] = (double*) malloc(Cols * sizeof (double));
        if (mat -> Values[i] == NULL) {
            MATNfree(mat);
            return NULL;
        }

    }

    return mat;
}

#define DO_SWAP(a, b, tmp)      { tmp = (a); (a) = (b); (b) = tmp; }

// Gauss-Jordan elimination. There is also a more
// exahustive non-singular matrix checking part.

BOOL MATNsolve(LPMATN a, LPMATN b)
{
    BOOL      status;
    int       n = a->Rows;
    int       i, iCol=0, iRow=0, j, k;
    double    fMax, fAbs, fSave, fInf, temp;
    int*      aiColIndex;
    int*      aiRowIndex=0;
    int*      aiPivoted=0;


    if (a->Rows != a->Cols) return FALSE;

    status = FALSE;
    if((aiColIndex = (int*) malloc(n * sizeof(int))) == NULL)
        goto GotError;

    if((aiRowIndex = (int*) malloc(n * sizeof(int))) == NULL)
        goto GotError;

    if((aiPivoted = (int*) malloc(n * sizeof(int))) == NULL)
        goto GotError;

    ZeroMemory(aiPivoted, n * sizeof(int));


    for(i = 0; i < n; i++) {

        // search matrix (excluding pivoted rows) for maximum absolute entry

        fMax = 0.0;
        for (j = 0; j < n; j++)
            if (aiPivoted[j] != 1)
                for (k = 0; k < n; k++)
                {
                   fAbs = fabs(a->Values[j][k]);
                   if (fAbs >= fMax) {

                        fMax = fAbs;
                        iRow = j;
                        iCol = k;
                    }
                    else
                    if (aiPivoted[k] > 1) {

                        status = FALSE;
                        goto GotError;
                    }
                }

        aiPivoted[iCol]++;

        // swap rows so that A[iCol][iCol] contains the pivot entry

        if (iRow != iCol) {

            for(j = 0; j < n; j++)
                DO_SWAP(a->Values[iRow][j], a->Values[iCol][j], temp)

            DO_SWAP(b->Values[iRow][0], b->Values[iCol][0], temp)
        }

        // keep track of the permutations of the rows

        aiRowIndex[i] = iRow;
        aiColIndex[i] = iCol;

        if (a->Values[iCol][iCol] == 0.0)
        {
            status = FALSE;
            goto GotError;
        }

       // scale the row so that the pivot entry is 1

        fInf = 1.0 / a->Values[iCol][iCol];
        a->Values[iCol][iCol] = 1.0;

                for(j = 0; j < n; j++)
                        a->Values[iCol][j] *= fInf;

                b->Values[iCol][0] *= fInf;

                // zero out the pivot column locations in the other rows

                for(j = 0; j < n; j++)
                        if (j != iCol) {

                                fSave = a->Values[j][iCol];
                                a->Values[j][iCol] = 0.0;

                                for(k = 0; k < n; k++)
                                        a->Values[j][k] -= a->Values[iCol][k] * fSave;

                                b->Values[j][0] -= b->Values[iCol][0] * fSave;
                        }
        }

        // reorder rows so that A[][] stores the inverse of the original matrix

        for(i = n - 1; i >= 0; i--) {

                if(aiRowIndex[i] != aiColIndex[i])
                        for(j = 0; j < n; j++)
                                DO_SWAP(a->Values[j][aiRowIndex[i]], a->Values[j][aiColIndex[i]], temp)
        }

        status = TRUE;

GotError:
        if(aiColIndex) free(aiColIndex);
        if(aiRowIndex) free(aiRowIndex);
        if(aiPivoted)  free(aiPivoted);
        return status;

}

#undef DO_SWAP


LPMATN MATNmult(LPMATN a1, LPMATN a2)
{
    int     i, j, k;
    LPMATN b;

    if (a1->Cols != a2->Rows)
        return NULL;

        b = MATNalloc (a1->Rows, a2->Cols);
        if (b == NULL)
                return NULL;

        for (i = 0; i < b->Rows; i++) {

                for (j = 0; j < b->Cols; j++) {

                        b->Values[i][j] = 0.0;

                        for (k = 0; k < a1->Cols; k++) {

                                b->Values[i][j] += a1->Values[i][k] * a2->Values[k][j];
                        }
                }
        }

        return b;
}


double MATNcross(LPMATN a)
{
        int i;
        double prod = 0.0;

        for (i = 0; i < a->Rows; i++) {

        prod += a->Values[i][0]*a->Values[i][0];
        }
        return prod;
}


void MATNscalar(LPMATN a, double scl, LPMATN b)
{
    int i, j;

    if (a->Rows != b->Rows || a->Cols != b->Cols)
        return;

    for (i = 0; i < a->Rows; i++) {

        for (j = 0; j < a->Cols; j++)
            b->Values[i][j] = a->Values[i][j] * scl;
    }
}


LPMATN MATNtranspose(LPMATN a)
{
    LPMATN b = MATNalloc(a->Cols, a->Rows);
    if (b != NULL) {

        int i, j;

        for (i = 0; i < a->Rows; i++)
        {
            for (j = 0; j < a->Cols; j++)
                b->Values[j][i] = a->Values [i][j];
        }
    }
    return b;
}



// Used for debug purposes
#ifdef DEBUG
void MATNprintf(char* name, LPMATN mat)
{
    int i, j;

    printf ("%s:\n", name);
    for (i= 0; i < mat->Rows; i++) {

        printf ("%3d", i);
        for (j = 0; j < mat->Cols; j++)
            printf ("  %.5f", mat->Values[i][j]);
        printf ("\n");
    }
}
#endif


