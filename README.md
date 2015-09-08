# RuteAction
VC++のソリューション一式です。  
DirectXのライブラリを使用しているのでWindows以外で動作させたい場合はWindowsの仮想環境等を利用してください。
  
また、ビルドには下記の手順1～3が必要です。

1.ソリューションに「Kojima_lib」プロジェクトの追加  
  （「Kojima_lib」プロジェクト自体は別のリポジトリに入っています。お手数ですが別途DLしてください。） 
  
2.「RuteAction」プロジェクトのプロパティにて「追加のインクルードディレクトリ」として「Kojima_lib」のソースフォルダを追加

3.「.NetFrameWork 」および「DirectX SDK」のインストール

  　動作確認バージョンは以下の通りです。  
  ・Windows 7 Professional  
  ・Visual Studio 2010 Professional  
  ・.NetFramework4.0＋  
  ・Microsoft DirectX SDK(August 2009)  
