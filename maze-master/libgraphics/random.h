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
 * 常数：RAND_MAX
 * ------------------
 *不幸的是，几个库据说符合
 * ANSI 标准未在 <stdlib.h> 中定义RAND_MAX。 
 * 为了减少可移植性问题，此接口定义RAND_MAX
 * 为最大正整数（如果未定义）。
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
 *功能：随机化
 * 用法： 随机化（）;
 * -------------------
 * 此功能设置随机种子，使随机序列
 * 是不可预测的。 在调试阶段，最好不要
 * 调用此函数，使程序行为可重复。
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
 * 功能：随机整数
 * 用法：n = 随机整数（低，高）;
 * ------------------------------------
 * 此函数返回一个从低到高的随机整数，
 *包容。
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
 *功能：随机真实
 * 用法： d = 随机实数（低、高）;
 * ---------------------------------
 * 此函数在半开中返回一个随机实数
 * 区间 [低 .. 高），表示结果始终是
 * 大于或等于低，但严格小于高。
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
 *功能：随机机会
 * 用法：如果（随机机会（p）） . . .
 * ---------------------------------
 * 随机机会函数返回 TRUE 和概率
 * 由 p 表示，它应该是介于
 * 0（表示从不）和 1（表示始终）。 例如，调用
 * RandomChance（.30） 在 30% 的时间内返回 TRUE。
 */

bool RandomChance(double p);

#endif
