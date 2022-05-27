#ifndef __PROGTEST__

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#endif /* __PROGTEST__ */

const int DAILY_CONSUMPTION = 292886;

int isLeap(int year) {
    return ((year % 400 == 0 || (year % 100 != 0 && year % 4 == 0)) && year % 4000 != 0);
}

int daysInMonth(int year, int month) {
    if (month == 4 || month == 6 || month == 9 || month == 11) return 30;
    if (month == 2) return 28 + isLeap(year);
    return 31;
}


int isValidDay(int year, int month, int day) {
    return (day >= 1 && day <= daysInMonth(year, month));
}

long long int toDays(int year, int month, int day) {
    long long day_num = 0;
    for (int i = 1; i < year; i++) day_num += 365 + isLeap(i);
    for (int i = 1; i < month; i++) day_num += daysInMonth(year, i);
    return day_num + day;
}

int consumptionOfSwitch(int state) {
    if (state == 59) return 5;
    if (state % 10 != 9) state = state % 10;
    else return consumptionOfSwitch((state / 10) % 10) + 2;
    if (state == 5 || state == 8) return 1;
    if (state == 3 || state == 4) return 3;
    if (state == 0 || state == 7) return 4;
    if (state == 1 || state == 6) return 5;
    return 2;
}

long long int gapConsumption(int start_hour, int start_minute, int target_hour, int target_minute) {
    long long int result = 0;
    while (!(start_minute == target_minute && start_hour == target_hour)) {
        result += consumptionOfSwitch(start_minute) + 200;
        start_minute += 1;
        if (start_minute == 60) {
            start_minute = 0;
            result += consumptionOfSwitch(start_hour);
            start_hour += 1;
        }
    }
    return result;
}

int energyConsumption(int y1, int m1, int d1, int h1, int i1,
                      int y2, int m2, int d2, int h2, int i2, long long int *consumption) {
    *consumption = 0;
    if (y1 < 1600 || y2 < 1600 || !(m1 >= 1 && m1 <= 12) || !(m2 >= 1 && m2 <= 12) || !(h1 >= 0 && h1 <= 23)
        || !(h2 >= 0 && h2 <= 23) || !(i1 >= 0 && i1 <= 59) || !(i2 >= 0 && i2 <= 59) || !isValidDay(y1, m1, d1) ||
        !isValidDay(y2, m2, d2) || 24*3600*toDays(y1, m1, d1)+h1*60+i1 > 24*3600*toDays(y2, m2, d2)+h2*60+i2)
        return 0;

    *consumption += (toDays(y2, m2, d2) - toDays(y1, m1, d1)) * DAILY_CONSUMPTION;
    if (*consumption == 0)
        *consumption += gapConsumption(h1, i1, h2, i2);
    else {
        *consumption += gapConsumption(0, 0, h2, i2);
        *consumption -= gapConsumption(0, 0, h1, i1);
    }
    return 1;
}

#ifndef __PROGTEST__

int main(int argc, char *argv[]) {
    long long int consumption;

    assert (energyConsumption(2021, 10, 1, 13, 15,
                              2021, 10, 1, 18, 45, &consumption) == 1
            && consumption == 67116LL);
    assert (energyConsumption(2021, 10, 1, 13, 15,
                              2021, 10, 2, 11, 20, &consumption) == 1
            && consumption == 269497LL);
    assert (energyConsumption(2021, 1, 1, 13, 15,
                              2021, 10, 5, 11, 20, &consumption) == 1
            && consumption == 81106033LL);
    assert (energyConsumption(2024, 1, 1, 13, 15,
                              2024, 10, 5, 11, 20, &consumption) == 1
            && consumption == 81398919LL);
    assert (energyConsumption(1900, 1, 1, 13, 15,
                              1900, 10, 5, 11, 20, &consumption) == 1
            && consumption == 81106033LL);
    assert (energyConsumption(2021, 10, 1, 0, 0,
                              2021, 10, 1, 12, 0, &consumption) == 1
            && consumption == 146443LL);
    assert (energyConsumption(2021, 10, 1, 0, 15,
                              2021, 10, 1, 0, 25, &consumption) == 1
            && consumption == 2035LL);
    assert (energyConsumption(2021, 10, 1, 12, 0,
                              2021, 10, 1, 12, 0, &consumption) == 1
            && consumption == 0LL);
    assert (energyConsumption(2021, 10, 1, 12, 0,
                              2021, 10, 1, 12, 1, &consumption) == 1
            && consumption == 204LL);
    assert (energyConsumption(2021, 11, 1, 12, 0,
                              2021, 10, 1, 12, 0, &consumption) == 0);
    assert (energyConsumption(2021, 10, 32, 12, 0,
                              2021, 11, 10, 12, 0, &consumption) == 0);
    assert (energyConsumption(2100, 2, 29, 12, 0,
                              2100, 2, 29, 12, 0, &consumption) == 0);
    assert (energyConsumption(2400, 2, 29, 12, 0,
                              2400, 2, 29, 12, 0, &consumption) == 1
            && consumption == 0LL);

    return 0;
}

#endif /* __PROGTEST__ */
