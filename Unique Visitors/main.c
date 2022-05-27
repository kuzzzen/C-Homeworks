#include <stdio.h>
#include <stdlib.h>

int main() {
    int visits[1000000], visit_counter[1000001];
    for (int i = 0; i < 100001; i++) visit_counter[i] = 0;
    char sign;
    int id, from, to, visit_count = 0;
    printf("Requests:\n");
    while (1) {
        int input_sign = scanf(" %c ", &sign);
        if (input_sign == EOF) break;
        if (sign != '+' && sign != '?') {
            printf("Invalid input.\n");
            return 1;
        }
        if (sign == '+') {
            int input = scanf("%d", &id);
            if (id < 0 || id > 99999 || visit_count > 1000000 || input != 1) {
                printf("Invalid input.\n");
                return 1;
            }
            visits[visit_count] = id;
            visit_counter[id] += 1;
            visit_count++;
            if (visit_counter[id] == 1) printf("> first visit\n");
            else printf("> visit #%d\n", visit_counter[id]);
        } else if (sign == '?') {
            int input = scanf("%d %d", &from, &to);
            if (from < 0 || to >= visit_count || input != 2 || to < from) {
                printf("Invalid input.\n");
                return 1;
            }
            int *set = (int *) malloc(100000 * sizeof(int)), count = 0;
            for (int i = 0; i < 100000; i++) set[i] = 0;
            for (int i = from; i <= to; i++) {
                if (set[visits[i]] == 0) {
                    count += 1;
                    set[visits[i]] = 1;
                }
            }
            free(set);
            printf("> %d / %d\n", count, to - from + 1);
        }


    }

    return 0;
}
