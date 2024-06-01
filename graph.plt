set term pdfcairo size 12in, 8in font "VL-PGothic-Regular.ttf,24"
set output "./graph/graph.pdf"

# 縦軸の最大値を求める処理
stats './data/p_distr.dat' using 2
ymax1 = STATS_max
x = STATS_records

stats './data/histgram1.dat' using 2
ymax2 = STATS_max

stats './data/histgram2.dat' using 2
ymax3 = STATS_max

stats './data/histgram3.dat' using 2
ymax4 = STATS_max

ymax = ymax1 > ymax2 ? ymax1 : ymax2
ymax = ymax > ymax3 ? ymax : ymax3
ymax = ymax > ymax4 ? ymax : ymax4

unset key
set multiplot layout 2,2
set yrange[0:ymax*1.1]
set xlabel "状態"
set ylabel "確率"

set title "ボルツマンマシンの確率分布"
plot "./data/p_distr.dat" smooth freq with boxes fill solid title "確率分布" lc rgb "#006400"

set title "1000回サンプリングしたデータの出現確率"
plot "./data/histgram1.dat" smooth freq with boxes fill solid title "出現確率" lc rgb "#006400"

set title "5000回サンプリングしたデータの出現確率"
plot "./data/histgram2.dat" smooth freq with boxes fill solid title "出現確率" lc rgb "#006400"

set title "10000回サンプリングしたデータの出現確率"
plot "./data/histgram3.dat" smooth freq with boxes fill solid title "出現確率" lc rgb "#006400"

unset multiplot

set term pdfcairo size 6in, 4in font "VL-PGothic-Regular.ttf,24"
set output "./graph/graph2.pdf"

set key default
set title "出現確率と確率分布の比較"
plot "./data/histgram1.dat" w l title "出現確率1", "./data/histgram2.dat" w l title "出現確率2", "./data/histgram3.dat" w l title "出現確率3", "./data/p_distr.dat" w l title "確率分布"