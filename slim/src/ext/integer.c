/* 整数関連のコールバック */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include "../lmntal_ext.h"
#include "../special_atom.h"

void init_integer(void);

/**
 * ($start, $end, $g)
 * where
 *  start, end = integer
 *  g = ground
 *
 * Creates a (multi)set $g[$a], $g[$a+1], ..., $g[$b].
 */
void integer_set(LmnMembrane *mem,
                 LmnWord a0, LmnLinkAttr t0,
                 LmnWord a1, LmnLinkAttr t1,
                 LmnWord a2, LmnLinkAttr t2)
{
  int i, j, n;
  int start = (int)a0;
  int end = (int)a1;
  Vector *srcvec = vec_make(16);

  vec_push(srcvec, (LmnWord)LinkObj_make(a2, t2));

  for (i = 0, n = start; n <= end; i++, n++) {
    Vector *dstlovec;
    SimpleHashtbl *atommap;

    lmn_mem_copy_ground(mem, srcvec, &dstlovec, &atommap);
    
    LinkObj l = (LinkObj)vec_get(dstlovec, 0);
    lmn_mem_newlink(mem, n, LMN_INT_ATTR, 0,
                    l->ap, t2, LMN_ATTR_GET_VALUE(l->pos));
                    
    for (j = 0; j < vec_num(dstlovec); j++) LMN_FREE(vec_get(dstlovec, j));
    vec_free(dstlovec);
    hashtbl_free(atommap);
  }

  lmn_mem_remove_atom(mem, a0, t0);
  lmn_mem_remove_atom(mem, a1, t1);
  lmn_free_atom(a0, t0);
  lmn_free_atom(a1, t1);

  lmn_mem_remove_ground(mem, srcvec);
  lmn_mem_free_ground(srcvec);

  for (i = 0; i < vec_num(srcvec); i++) LMN_FREE(vec_get(srcvec, i));
  vec_free(srcvec);
}


/*
 * (N):
 * 
 * sets N as the seed for random numbers
 */
void integer_srand(LmnMembrane *mem,
                   LmnWord a0, LmnLinkAttr t0)
{
  srand(a0);
  lmn_mem_remove_atom(mem, a0, t0);
  lmn_free_atom(a0, t0);
}


/*
 * (N, H):
 * 
 * H is bound to a random number between 0 and N-1.
 */
void integer_rand(LmnMembrane *mem,
                  LmnWord a0, LmnLinkAttr t0,
                  LmnWord a1, LmnLinkAttr t1)
{
  LmnWord n = rand() % a0;

  lmn_mem_newlink(mem,
                  a1, LMN_ATTR_MAKE_LINK(0), LMN_ATTR_GET_VALUE(t1),
                  n, LMN_INT_ATTR, 0);
  lmn_mem_push_atom(mem, n, LMN_INT_ATTR);

  lmn_mem_remove_atom(mem, a0, t0);
  lmn_free_atom(a0, t0);
}

/*
 * (N, H):
 * 
 * H is bound to a random number between 0 and N-1.
 */
void integer_of_string(LmnMembrane *mem,
                       LmnWord a0, LmnLinkAttr t0,
                       LmnWord a1, LmnLinkAttr t1)
{
  const char *s = (const char *)LMN_SP_ATOM_DATA(a0);
  char *t = NULL;
  long n;

  n = strtol(s, &t, 10);
  if (t == NULL || s == t) {
    LmnAtomPtr a = lmn_mem_newatom(mem, lmn_functor_intern(ANONYMOUS,
                                                           lmn_intern("fail"),
                                                           1));
    lmn_mem_newlink(mem,
                    a1, t1, LMN_ATTR_GET_VALUE(t1),
                    (LmnWord)a, LMN_ATTR_MAKE_LINK(0), 0);
  } else { /* 変換できた */
    printf("%p %p\n", s, t);
    lmn_mem_newlink(mem,
                    a1, t1, LMN_ATTR_GET_VALUE(t1),
                    n, LMN_INT_ATTR, 0);
    lmn_mem_push_atom(mem, n, LMN_INT_ATTR);
  }

  lmn_mem_remove_atom(mem, a0, t0);
  lmn_free_atom(a0, t0);
}

void init_integer(void)
{
  lmn_register_c_fun("integer_set", integer_set, 3);
  lmn_register_c_fun("integer_srand", integer_srand, 1);
  lmn_register_c_fun("integer_rand", integer_rand, 2);
  lmn_register_c_fun("integer_of_string", integer_of_string, 2);

  srand((unsigned)time(NULL));
}
