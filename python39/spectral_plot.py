import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

def read_spectral_data(filename):
    """
    读取USGS光谱库数据文件
    Args:
        filename: 光谱数据文件路径
    Returns:
        spectral: 归一化后的光谱数据
        description: 光谱数据描述信息
    """
    # 读取第一行描述信息
    with open(filename, 'r') as f:
        description = f.readline().strip()
    
    # 读取数据，跳过第一行（描述行）
    spectral_data = pd.read_csv(filename, skiprows=1, header=None)
    spectral = spectral_data.values.flatten()  # 将数据转换为一维数组
    
    # 移除无效数据（如 -1.2300000e+034）
    spectral = spectral[spectral > -1e6]  # 过滤掉异常值
    
    # 归一化处理
    spectral = (spectral - np.min(spectral)) / (np.max(spectral) - np.min(spectral))
    
    return spectral, description

def plot_spectral(spectral, description):
    """
    绘制光谱数据
    Args:
        spectral: 光谱数据
        description: 光谱数据描述信息
    """
    plt.figure(figsize=(12, 6))
    
    # 创建x轴数据（波段索引）
    x = np.arange(len(spectral))
    
    # 绘制光谱曲线
    plt.plot(x, spectral, '-', linewidth=1)
    
    plt.xlabel('Bands')
    plt.ylabel('Relative spectral response')
    plt.title(description)
    plt.grid(True)
    
    # 添加数据信息
    info_text = f'数据点数: {len(spectral)}\n'
    info_text += f'最大值: {np.max(spectral):.4f}\n'
    info_text += f'最小值: {np.min(spectral):.4f}'
    plt.text(0.02, 0.98, info_text, 
             transform=plt.gca().transAxes, 
             verticalalignment='top',
             bbox=dict(boxstyle='round', facecolor='white', alpha=0.8))
    
    # 设置x轴刻度
    plt.xticks(np.arange(0, len(spectral), 100))  # 每100个波段显示一个刻度
    
    plt.tight_layout()
    plt.show()

def main():
    # 文件路径
    filename = r"E:\DataSet\usgs_splib07\ASCIIdata\ASCIIdata_splib07a\ChapterM_Minerals\splib07a_Actinolite_HS22.1B_ASDFRb_AREF.txt"
    
    try:
        # 读取数据
        spectral, description = read_spectral_data(filename)
        
        # 打印描述信息
        print("光谱数据描述:", description)
        print(f"数据点数: {len(spectral)}")
        print(f"数据范围: [{np.min(spectral):.4f}, {np.max(spectral):.4f}]")
        
        # 绘制光谱
        plot_spectral(spectral, description)
        
    except Exception as e:
        print(f"错误: {str(e)}")

if __name__ == "__main__":
    main() 