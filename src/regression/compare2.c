//#include "p16c84.h"

unsigned char success = 0;
unsigned char failures = 0;
unsigned char dummy = 0;

bit bit0 = 0;
unsigned int aint0 = 0;
unsigned int aint1 = 0;
unsigned char achar0 = 0;
unsigned char achar1 = 0;

void
done ()
{

  dummy++;

}



void
char_lt_char (void)
{
  if (achar0 < achar1)
    failures++;
}

void
char_gt_char (void)
{
  if (achar1 > achar0)
    failures++;
}

void
char_lte_char (void)
{
  if (achar0 <= achar1)
    failures++;
}

void
char_gte_char (void)
{
  if (achar1 >= achar0)
    failures++;
}

void
char_lt_lit (void)
{
  if (achar1 < 0x10)
    failures++;
}

void
char_gt_lit (void)
{
  if (achar1 > 0x10)
    failures++;
}

void
char_lte_lit (void)
{
  if (achar1 <= 0x0f)
    failures++;
}

void
char_gte_lit (void)
{
  if (achar1 >= 0x11)
    failures++;
}


/* ints */

void
int_lt_int (void)
{
  if (aint0 < aint1)
    failures++;
}

void
int_gt_int (void)
{
  if (aint1 > aint0)
    failures++;
}

void
int_lte_int (void)
{
  if (aint0 <= aint1)
    failures++;
}

void
int_gte_int (void)
{
  if (aint1 >= aint0)
    failures++;
}

void
int_lt_lit (void)
{
  if (aint1 < 0x10)
    failures++;
}

void
int_gt_lit (void)
{
  if (aint1 > 0x10)
    failures++;
}

void
int_lte_lit (void)
{
  if (aint1 <= 0x0f)
    failures++;
}

void
int_gte_lit (void)
{
  if (aint1 >= 0x11)
    failures++;
}







void
main (void)
{

  char_lt_char ();
  char_gt_char ();

  achar0++;
  char_lt_char ();
  char_gt_char ();
  char_gte_char ();
  char_lte_char ();

  achar1 = 0x10;
  char_lt_lit ();
  char_gt_lit ();
  char_lte_lit ();
  char_gte_lit ();



  int_lt_int ();
  int_gt_int ();

  aint0++;
  int_lt_int ();
  int_gt_int ();
  int_gte_int ();
  int_lte_int ();

  aint1 = 0x10;
  int_lt_lit ();
  int_gt_lit ();

  success = failures;
  done ();
}
