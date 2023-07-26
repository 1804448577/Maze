/*
 * File: random.h
 * Version: 1.0
 * Last modified on Fri Jul 22 16:44:36 1994 by eroberts
 * -----------------------------------------------------
 * This interface provides several functions for generating
 * pseudo-random numbers.
 */

#ifndef _random_h
#define _random_h

#include "genlib.h"
#include <stdlib.h>

/*
 * Constant: RAND_MAX
 * ------------------
 * Unfortunately, several libraries that supposedly conform to
 * the ANSI standard do not define RAND_MAX in <stdlib.h>.  To
 * reduce portability problems, this interface defines RAND_MAX
 * to be the largest positive integer if it is undefined.
 */
 
/*
 * ������RAND_MAX
 * ------------------
 *���ҵ��ǣ��������˵����
 * ANSI ��׼δ�� <stdlib.h> �ж���RAND_MAX�� 
 * Ϊ�˼��ٿ���ֲ�����⣬�˽ӿڶ���RAND_MAX
 * Ϊ��������������δ���壩��
 */

#ifndef RAND_MAX
#  define RAND_MAX ((int) ((unsigned) ~0 >> 1))
#endif

/*
 * Function: Randomize
 * Usage: Randomize();
 * -------------------
 * This function sets the random seed so that the random sequence
 * is unpredictable.  During the debugging phase, it is best not
 * to call this function, so that program behavior is repeatable.
 */
 
/*
 *���ܣ������
 * �÷��� ���������;
 * -------------------
 * �˹�������������ӣ�ʹ�������
 * �ǲ���Ԥ��ġ� �ڵ��Խ׶Σ���ò�Ҫ
 * ���ô˺�����ʹ������Ϊ���ظ���
 */

void Randomize(void);

/*
 * Function: RandomInteger
 * Usage: n = RandomInteger(low, high);
 * ------------------------------------
 * This function returns a random integer in the range low to high,
 * inclusive.
 */
 
 /*
 * ���ܣ��������
 * �÷���n = ����������ͣ��ߣ�;
 * ------------------------------------
 * �˺�������һ���ӵ͵��ߵ����������
 *���ݡ�
 */
 

int RandomInteger(int low, int high);

/*
 * Function: RandomReal
 * Usage: d = RandomReal(low, high);
 * ---------------------------------
 * This function returns a random real number in the half-open
 * interval [low .. high), meaning that the result is always
 * greater than or equal to low but strictly less than high.
 */

/*
 *���ܣ������ʵ
 * �÷��� d = ���ʵ�����͡��ߣ�;
 * ---------------------------------
 * �˺����ڰ뿪�з���һ�����ʵ��
 * ���� [�� .. �ߣ�����ʾ���ʼ����
 * ���ڻ���ڵͣ����ϸ�С�ڸߡ�
 */

double RandomReal(double low, double high);

/*
 * Function: RandomChance
 * Usage: if (RandomChance(p)) . . .
 * ---------------------------------
 * The RandomChance function returns TRUE with the probability
 * indicated by p, which should be a floating-point number between
 * 0 (meaning never) and 1 (meaning always).  For example, calling
 * RandomChance(.30) returns TRUE 30 percent of the time.
 */
 
/*
 *���ܣ��������
 * �÷��������������ᣨp���� . . .
 * ---------------------------------
 * ������ắ������ TRUE �͸���
 * �� p ��ʾ����Ӧ���ǽ���
 * 0����ʾ�Ӳ����� 1����ʾʼ�գ��� ���磬����
 * RandomChance��.30�� �� 30% ��ʱ���ڷ��� TRUE��
 */

bool RandomChance(double p);

#endif
