#include "bm.h"

int main(void){
    BM bm(10);
    int num = 1000;
    cout << "start p_distr_calc()" << endl;
    bm.p_distr_calc();
    cout << "end p_distr_calc()" << endl;

    FILE *p, *h;
    int i;
    p = fopen("./data/p_distr.dat", "w");
    if (p != NULL) {
        for(i=0;i<bm.totalStates;i++){
            fprintf(p, "%d %lf\n", i, bm.p_distr[i]); // ファイルにテキストを書き込む
        }
        fclose(p); // ファイルを閉じる
    } else {
        perror("Error opening p"); // ファイルを開けなかった場合のエラー処理
        return 1;
    }

    cout << "start sampling: " << num << endl;
    bm.sampling(num);
    cout << "end sampling" << endl;
    h = fopen("./data/histgram1.dat", "w");
    if (h != NULL) {
        for(i=0;i<bm.totalStates;i++){
            fprintf(h, "%d %lf\n", i, (double)bm.histgram[i]/num); // ファイルにテキストを書き込む
        }
        fclose(h); // ファイルを閉じる
    } else {
        perror("Error opening h"); // ファイルを開けなかった場合のエラー処理
        return 1;
    }

    num = 5000;
    cout << "start sampling: " << num << endl;
    bm.sampling(num);
    cout << "end sampling" << endl;
    h = fopen("./data/histgram2.dat", "w");
    if (h != NULL) {
        for(i=0;i<bm.totalStates;i++){
            fprintf(h, "%d %lf\n", i, (double)bm.histgram[i]/num); // ファイルにテキストを書き込む
        }
        fclose(h); // ファイルを閉じる
    } else {
        perror("Error opening h"); // ファイルを開けなかった場合のエラー処理
        return 1;
    }

    num = 10000;
    cout << "start sampling: " << num << endl;
    bm.sampling(num);
    cout << "end sampling" << endl;
    h = fopen("./data/histgram3.dat", "w");
    if (h != NULL) {
        for(i=0;i<bm.totalStates;i++){
            fprintf(h, "%d %lf\n", i, (double)bm.histgram[i]/num); // ファイルにテキストを書き込む
        }
        fclose(h); // ファイルを閉じる
    } else {
        perror("Error opening h"); // ファイルを開けなかった場合のエラー処理
        return 1;
    }

    return 0;
}