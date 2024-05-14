#include<iostream>
#include<cmath>
#include<random>
#include<cstdio>
using namespace std; 

#ifndef   BM_H
#define   BM_H
class BM {
    public:
        double *p_distr; // 確率分布の厳密解
        int *histgram; // 経験分布
        int totalStates; // 2^Nの値を保存する変数
        int* update(); // ギブスサンプリングで状態を一回更新する関数
        void sampling(int n); // ギブスサンプリングをn回行い、histgramに結果を代入する
        void p_distr_calc(); // 確率分布の厳密解を計算する関数
        void dataGen(int num); // 訓練データを生成する関数
        void dataRead(int num); // 訓練データを読み込む関数
        void train(); // 読み込んだ訓練データにBMを学習させる関数
        double random_num(); // 乱数生成器 [0,1]の実数
        BM(int n); // コンストラクタ
        ~BM(); // デコンストラクタ
    private:
        int N; // 確率変数の数
        int *X; // 確率変数
        double **w; // 結合の重み
        double *b; // バイアス
        int **traindata; // 訓練データを保存する配列
        int traindatanum; // 訓練データの数
        double energy_calc(); // BMの現在の状態でのエネルギーを計算する関数
        double lambda(int i);
        double sig(double x); // シグモイド関数
        int x_num(); // 現在の状態を2進数で返す関数
        void setX(int num); // 2進数から状態Xを設定する関数
        double learn_rate;

        // 乱数生成器のメンバ変数
        std::mt19937 gen;
        std::uniform_real_distribution<double> dis;
};

#endif