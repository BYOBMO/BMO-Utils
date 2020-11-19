
#include "pca9685.h"

#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PIN_BASE 300
#define MAX_PWM 4096
#define HERTZ 50

#define MIN_MILLI 0.65
#define MAX_MILLI 2.55

int leftMin = 60;
int leftMax = 160;
int rightMin = 20;
int rightMax = 130;

#define VERSION "v1.2"

void getLimits()
{
	FILE *f;
	int min, max;
	char str[100];

	f = fopen("/home/pi/bmos/servos/limits.txt" ,"r");
	if (f!=NULL)
	{
		fscanf(f, "%s %d %d", str, &min, &max);
		if (strcmp(str, "left:") == 0)
		{
			leftMin = min;
			leftMax = max;
		}
		else if (strcmp(str, "right:") == 0)
		{
			rightMin = min;
			rightMax = max;
		}

		fscanf(f, "%s %d %d", str, &min, &max);
		if (strcmp(str, "left:") == 0)
		{
			leftMin = min;
			leftMax = max;
		}
		else if (strcmp(str, "right:") == 0)
		{
			rightMin = min;
			rightMax = max;
		}

		printf("left %d %d\n", leftMin, leftMax);		
		printf("right %d %d\n", rightMin, rightMax);
		fclose(f);
	}
	else
	{
		printf("Can't open limit file.\n");
	}
}



float millisFromAngle(float angle)
{
	float milli;
	float range = MAX_MILLI - MIN_MILLI + .01;
	float percent = (float)angle / 180.0;

	milli = MIN_MILLI + (range * percent);

	return(milli);

}
/**
 * Calculate the number of ticks the signal should be high for the required amount of time
 */
int calcTicks(float impulseMs, int hertz)
{
	float cycleMs = 1000.0f / hertz;
	return (int)(MAX_PWM * impulseMs / cycleMs + 0.5f);
}

/**
 * input is [0..1]
 * output is [min..max]
 */
float map(float input, float min, float max)
{
	return (input * max) + (1 - input) * min;
}

int leftCheck(int val)
{
	float millis;
	int minTick, maxTick;

	millis = millisFromAngle(leftMax);
	maxTick = calcTicks(millis, HERTZ);

	millis = millisFromAngle(leftMin);
	minTick = calcTicks(millis, HERTZ);

	if (val < minTick)
		val = minTick;
	else if (val > maxTick)
		val = maxTick;

	return(val);
}

int rightCheck(int val)
{
	float millis;
	int minTick, maxTick;

	millis = millisFromAngle(rightMax);
	maxTick = calcTicks(millis, HERTZ);

	millis = millisFromAngle(rightMin);
	minTick = calcTicks(millis, HERTZ);

	if (val < minTick)
		val = minTick;
	else if (val > maxTick)
		val = maxTick;

	return(val);
}

void waveleft(int del)
{
	int i;
	float millis;
	int tick;

	int minTick, maxTick;

	millis = millisFromAngle(leftMax);
	maxTick = calcTicks(millis, HERTZ);

	millis = millisFromAngle(leftMax-60);
	minTick = calcTicks(millis, HERTZ);

	printf("%d, %d", minTick, maxTick);

	for (i = maxTick; i >= minTick; i -= 5)
	{
		pwmWrite(PIN_BASE + 1, i);

		delay(del);
	}

	for (i = minTick; i <= maxTick; i += 5)
	{
		pwmWrite(PIN_BASE + 1, i);

		delay(del);

	}


}

void waveright(int del)
{
	int i;
	float millis;
	int tick;

	int minTick, maxTick;

	millis = millisFromAngle(rightMin);
	minTick = calcTicks(millis, HERTZ);

	millis = millisFromAngle(rightMin+50);
	maxTick = calcTicks(millis, HERTZ);

	for (i = minTick; i <= maxTick; i += 5)
	{
		pwmWrite(PIN_BASE + 0, i);

		delay(del);
	}

	for (i = maxTick; i >= minTick; i -= 5)
	{
		pwmWrite(PIN_BASE + 0, i);

		delay(del);
	}
}

void waveboth(int del)
{
	int i;
	float millis;
	int tick;
	int minTickL, maxTickL;
	int minTickR, maxTickR;

	millis = millisFromAngle(leftMax-60);
	minTickL = calcTicks(millis, HERTZ);

	millis = millisFromAngle(leftMax);
	maxTickL = calcTicks(millis, HERTZ);

	millis = millisFromAngle(rightMin + 60);
	maxTickR = calcTicks(millis, HERTZ);

	millis = millisFromAngle(rightMin);
	minTickR = calcTicks(millis, HERTZ);


	for (i = minTickR; i <= maxTickR; i += 5)
	{
		pwmWrite(PIN_BASE + 0, i);
		pwmWrite(PIN_BASE + 1, minTickL + (i- minTickR));

		delay(del);
	}

	for (i = maxTickR; i >= minTickR; i -= 5)
	{
		pwmWrite(PIN_BASE + 0, i);
		pwmWrite(PIN_BASE + 1, maxTickL + (i - maxTickR));

		delay(del);
	}
}

void move2(int rightPin, int toTick1, int leftPin, int toTick2, int del)
{
	int curTick1 = rightCheck(digitalRead(rightPin) & 0xFFF);
	int curTick2 = leftCheck(digitalRead(leftPin) & 0xFFF);

	int i1, i2;;
	int inc1 = 5;
	int inc2 = 5;


	if (curTick1 > toTick1)
	{
		inc1 = -5;
	}

	if (curTick2 > toTick2)
	{
		inc2 = -5;
	}

	i1 = curTick1;
	i2 = curTick2;

	while(i1!=toTick1 || i2!=toTick2)
	{
		i1 += inc1;
		i2 += inc2;

		if ((inc1<0 && i1<toTick1) || (inc1>0&&i1>toTick1))
		{
			i1 = toTick1;
		}

		if ((inc2<0 && i2<toTick2) || (inc2>0 && i2>toTick2))
		{
			i2 = toTick2;
		}
		pwmWrite(rightPin, i1);
		pwmWrite(leftPin, i2);

		delay(del);
	}
		
}

void move(int pin, int toTick, int del)
{
	int curTick = digitalRead(pin) & 0xFFF;

	if (pin == PIN_BASE + 0)
	{
		curTick = rightCheck(curTick);
	}
	else if (pin == PIN_BASE + 1)
	{
		curTick = leftCheck(curTick);
	}

	int i;
	int inc = 5;

	if (curTick>toTick)
	{
		for (i=curTick; i>=toTick; i = i-inc)
		{
			pwmWrite(pin, i);
			delay(del);
		}
	}
	else
	{
		for (i=curTick; i<=toTick; i = i+inc)
		{
			pwmWrite(pin, i);
			delay(del);
		}
	}

	pwmWrite(pin, toTick);
		
}

int main(int argc, char** argv)
{
	getLimits();

	float millis, millis1;
	int tick, tick1;
	int val = -1;
	char arm='x';
	char dir = 'x';
	int del = 7;
	char c;

	if (argc>1)
	{
		arm =argv[1][0];
		if (argc > 2)
		{
			c = argv[2][0];
			if (c == 'u' || c == 'd' || c=='w')
			{
				dir = c;
			}
			else
			{
				val = atoi(argv[2]);
			}
		}

		if (argc>3)
		{
			del = atoi(argv[3]);
		}
	}
	else
	{
		printf("%s\n", VERSION);
	}

	// Calling wiringPi setup first.
	wiringPiSetup();

	// Setup with pinbase 300 and i2c location 0x40
	int fd = pca9685Setup(PIN_BASE, 0x40, HERTZ);
	if (fd < 0)
	{
		printf("Error in setup\n");
		return fd;
	}

	// Reset all output
	//pca9685PWMReset(fd);


	// Set servo to neutral position at 1.5 milliseconds
	// (View http://en.wikipedia.org/wiki/Servo_control#Pulse_duration)
	if (arm == 'b')
	{
		if (dir == 'w')
		{
			int saveTick1 = rightCheck(digitalRead(PIN_BASE + 0) & 0xFFF);
			int saveTick2 = leftCheck(digitalRead(PIN_BASE + 1) & 0xFFF);

			millis = millisFromAngle(rightMin);
			tick = calcTicks(millis, HERTZ);
			millis1 = millisFromAngle(leftMax);
			tick1 = calcTicks(millis1, HERTZ);


			millis = millisFromAngle(leftMax-60);
			int minTickL = calcTicks(millis, HERTZ);
			move2(PIN_BASE + 0, tick, PIN_BASE + 1, tick1, del);
			move2(PIN_BASE + 0, tick, PIN_BASE + 1, minTickL, del);

			waveboth(del);
			waveboth(del);
			waveboth(del);

			delay(250);
			move2(PIN_BASE + 0, saveTick1, PIN_BASE + 1, saveTick2, del);

		}
		else
		{
			if (dir == 'u')
			{
				millis = millisFromAngle(rightMin);
				tick = calcTicks(millis, HERTZ);
				millis1 = millisFromAngle(leftMax);
				tick1 = calcTicks(millis1, HERTZ);


				move2(PIN_BASE + 0, tick, PIN_BASE + 1, tick1, del);
			}
			else if (dir == 'd')
			{
				millis = millisFromAngle(rightMax);
				tick = calcTicks(millis, HERTZ);
				millis1 = millisFromAngle(leftMin);
				tick1 = calcTicks(millis1, HERTZ);

				move2(PIN_BASE + 0, tick, PIN_BASE + 1, tick1, del);
			}
		}
	}
	else if (arm == 'r')
	{
		if (dir == 'w')
		{
			int saveTick = rightCheck(digitalRead(PIN_BASE + 0) & 0xFFF);

			millis = millisFromAngle(rightMin);
			tick = calcTicks(millis, HERTZ);
			move(PIN_BASE + 0, tick, del);

			waveright(del);
			waveright(del);
			waveright(del);

			delay(250);
			move(PIN_BASE + 0, saveTick, del);
		}
		else
		{
			if (dir == 'u')
			{
				val = rightMin;
			}
			else if (dir == 'd')
			{
				val = rightMax;
			}

			if (val < rightMin)
				val = rightMin;
			else if (val > rightMax)
				val = rightMax;

			millis = millisFromAngle(val);
			tick = calcTicks(millis, HERTZ);

			move(PIN_BASE + 0, tick, del);
		}
	}
	else if (arm == 'l')
	{
		if (dir == 'w')
		{
			int saveTick = leftCheck(digitalRead(PIN_BASE + 1) & 0xFFF);

			millis = millisFromAngle(leftMax);
			tick = calcTicks(millis, HERTZ);
			move(PIN_BASE + 1, tick, del);

			waveleft(del);
			waveleft(del);
			waveleft(del);

			delay(250);
			move(PIN_BASE + 1, saveTick, del);
		}
		else
		{
			if (dir == 'u')
			{
				val = leftMax;
			}
			else if (dir == 'd')
			{
				val = leftMin;
			}

			if (val < leftMin)
				val = leftMin;
			else if (val > leftMax)
				val = leftMax;

			millis = millisFromAngle(val);
			tick = calcTicks(millis, HERTZ);

			move(PIN_BASE + 1, tick, del);
		}
	}
	return 0;
}
