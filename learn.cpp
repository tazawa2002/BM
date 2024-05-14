#include "bm.h"

int main(void){
    int n = 6;
    int num = 10000;
    BM bm1(n);
    BM bm2(n);

    bm1.dataGen(num); // 訓練データを生成
    bm2.dataRead(num); // 訓練データを読み込む
    bm2.train(); // 群連データを用いて学習

    FILE *f;
    int i;
    bm1.p_distr_calc();
    f = fopen("./graph/bm1_p.txt", "w");
    if (f != NULL) {
        for(i=0;i<bm1.totalStates;i++){
            fprintf(f, "%d %lf\n", i, bm1.p_distr[i]); // ファイルにテキストを書き込む
        }
        fclose(f); // ファイルを閉じる
    } else {
        perror("Error opening f"); // ファイルを開けなかった場合のエラー処理
        return 1;
    }

    bm2.p_distr_calc();
    f = fopen("./graph/bm2_p.txt", "w");
    if (f != NULL) {
        for(i=0;i<bm2.totalStates;i++){
            fprintf(f, "%d %lf\n", i, bm2.p_distr[i]); // ファイルにテキストを書き込む
        }
        fclose(f); // ファイルを閉じる
    } else {
        perror("Error opening f"); // ファイルを開けなかった場合のエラー処理
        return 1;
    }

    f = fopen("./graph/bm1_q.txt", "w");
    if (f != NULL) {
        for(i=0;i<bm1.totalStates;i++){
            fprintf(f, "%d %lf\n", i, (double)bm1.histgram[i]/num); // ファイルにテキストを書き込む
        }
        fclose(f); // ファイルを閉じる
    } else {
        perror("Error opening f"); // ファイルを開けなかった場合のエラー処理
        return 1;
    }

    bm2.sampling(num);
    f = fopen("./graph/bm2_q.txt", "w");
    if (f != NULL) {
        for(i=0;i<bm2.totalStates;i++){
            fprintf(f, "%d %lf\n", i, (double)bm2.histgram[i]/num); // ファイルにテキストを書き込む
        }
        fclose(f); // ファイルを閉じる
    } else {
        perror("Error opening f"); // ファイルを開けなかった場合のエラー処理
        return 1;
    }

    return 0;
}