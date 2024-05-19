set term pdfcairo size 12in, 8in font "VL-PGothic-Regular.ttf,24"
set output "./graph/learn.pdf"
set grid

# 縦軸の最大値を求める処理
stats './data/bm1_p.dat' using 2
ymax1 = STATS_max
x = STATS_records

stats './data/bm2_p.dat' using 2
ymax2 = STATS_max

stats './data/bm1_q.dat' using 2
ymax3 = STATS_max

stats './data/bm2_q.dat' using 2
ymax4 = STATS_max

ymax = ymax1 > ymax2 ? ymax1 : ymax2
# ymax = ymax > ymax3 ? ymax : ymax3
# ymax = ymax > ymax4 ? ymax : ymax4

unset key
set multiplot layout 2,2 spacing 0.5,0.5
set yrange [0:ymax*1.1]
set xrange [-0.5:x-0.5]
set xlabel "状態"
set ylabel "確率"

set title "生成モデルの確率分布"
plot "./data/bm1_p.dat" with boxes fill solid lc rgb "#006400"

set title "学習モデルの確率分布"
plot "./data/bm2_p.dat" with boxes fill solid lc rgb "#006400"

unset yrange
set ylabel "出現回数"
set title "生成モデルのサンプリング結果"
plot "./data/bm1_q.dat" with boxes fill solid lc rgb "#006400"
# plot "./data/data.dat" smooth freq with boxes fill solid

set title "学習モデルのサンプリング結果"
plot "./data/bm2_q.dat" with boxes fill solid lc rgb "#006400"

unset multiplot

set term pdfcairo size 6in, 4in font "VL-PGothic-Regular.ttf,24"
set output "./graph/learn2.pdf"

set key default
set title "生成モデルと学習モデルの確率分布の比較"
plot "./data/bm1_p.dat" w l title "生成モデル", "./data/bm2_p.dat" w l title "学習モデル"