#ifndef SCORE_HPP
#define SCORE_HPP

typedef int int32_t;

struct Score {
	int32_t score = 0;
	int32_t hightScore = 0;
};

extern Score readScore();
extern void writeScore(int32_t hightScore);

#endif // !SCORE_HPP