set term pdfcairo size 12in, 8in font "VL-PGothic-Regular.ttf,24"
set output "./graph/graph.pdf"

unset key
set multiplot layout 2,2

set title "ボルツマンマシンの確率分布"
plot "./graph/p_distr.txt" smooth freq with boxes fill solid title "確率分布"

set title "N回サンプリングしたデータの出現確率"
plot "./graph/histgram1.txt" smooth freq with boxes fill solid title "出現確率"

set title "2N回サンプリングしたデータの出現確率"
plot "./graph/histgram2.txt" smooth freq with boxes fill solid title "出現確率"

set title "3N回サンプリングしたデータの出現確率"
plot "./graph/histgram3.txt" smooth freq with boxes fill solid title "出現確率"

unset multiplot

set term pdfcairo size 6in, 4in font "VL-PGothic-Regular.ttf,24"
set output "./graph/graph2.pdf"

set key default
set title "出現確率と確率分布の比較"
plot "./graph/histgram1.txt" w l title "出現確率1", "./graph/histgram2.txt" w l title "出現確率2", "./graph/histgram3.txt" w l title "出現確率3", "./graph/p_distr.txt" w l title "確率分布"