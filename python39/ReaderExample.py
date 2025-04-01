import HySpexLibrary
import numpy as np
import matplotlib.pyplot as plt
from pathlib import Path
import logging
import matplotlib

# 设置matplotlib中文字体
plt.rcParams['font.sans-serif'] = ['SimHei']  # 用来正常显示中文标签
plt.rcParams['axes.unicode_minus'] = False  # 用来正常显示负号

# 配置日志
logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')
logger = logging.getLogger(__name__)


class HySpexFileAnalyzer:
    def __init__(self):
        self.reader = None
        self.file_path = None
        self.properties = {}
        self.matrices = {}
        self.int_matrices = {}
        self.image_info = {}
        self.spatial_size = None
        self.spectral_size = None

    def open_file(self, file_path):
        """打开HYSPEX文件"""
        try:
            self.file_path = Path(file_path)
            if not self.file_path.exists():
                raise FileNotFoundError(f"文件不存在: {self.file_path}")

            logger.info(f"正在打开文件: {self.file_path}")
            self.reader = HySpexLibrary.FileReader()
            if not self.reader.open(str(self.file_path)):
                raise RuntimeError("无法打开文件")

            # 读取文件信息
            self._read_file_info()
            return True

        except Exception as e:
            logger.error(f"打开文件时出错: {str(e)}")
            self.close()
            return False

    def _read_file_info(self):
        """读取文件的所有信息"""
        try:
            # 1. 读取文件属性
            self._read_properties()

            # 2. 读取矩阵信息
            self._read_matrices()

            # 3. 读取图像信息
            self._read_image_info()

            # 4. 读取原始头部信息
            self._read_raw_header()

        except Exception as e:
            logger.error(f"读取文件信息时出错: {str(e)}")
            raise

    def _read_properties(self):
        """读取所有属性"""
        try:
            prop_count = self.reader.getPropertyKeyCount()
            logger.info(f"\n文件属性数量: {prop_count}")

            for i in range(prop_count):
                try:
                    key = self.reader.getPropertyKeyNameByIndex(i)
                    value = self.reader.getPropertyValue(key)
                    self.properties[key] = value
                    logger.info(f"属性: {key} = {value}")

                    # 保存关键属性
                    if key == "spatial_size":
                        self.spatial_size = int(value)
                    elif key == "spectral_size":
                        self.spectral_size = int(value)

                except Exception as e:
                    logger.warning(f"读取属性 {i} 时出错: {str(e)}")

        except Exception as e:
            logger.error(f"读取属性时出错: {str(e)}")

    def _read_matrices(self):
        """读取所有矩阵信息"""
        try:
            # 读取双精度矩阵
            double_count = self.reader.getDoubleMatrixCount()
            logger.info(f"\n双精度矩阵数量: {double_count}")

            for i in range(double_count):
                try:
                    key = self.reader.getDoubleMatrixKeyNameByIndex(i)
                    matrix = self.reader.getDoubleMatrix(key)
                    if matrix.size > 0:  # 检查矩阵是否为空
                        self.matrices[key] = {
                            'shape': matrix.shape,
                            'dtype': matrix.dtype,
                            'min': np.min(matrix),
                            'max': np.max(matrix),
                            'mean': np.mean(matrix)
                        }
                        logger.info(f"双精度矩阵 {key}:")
                        logger.info(f"  形状: {matrix.shape}")
                        logger.info(f"  数据类型: {matrix.dtype}")
                        logger.info(f"  数值范围: [{np.min(matrix)}, {np.max(matrix)}]")
                        logger.info(f"  平均值: {np.mean(matrix)}")
                    else:
                        logger.warning(f"双精度矩阵 {key} 为空")
                except Exception as e:
                    logger.warning(f"读取双精度矩阵 {i} 时出错: {str(e)}")

            # 读取整型矩阵
            int_count = self.reader.getIntMatrixCount()
            logger.info(f"\n整型矩阵数量: {int_count}")

            for i in range(int_count):
                try:
                    key = self.reader.getIntMatrixKeyNameByIndex(i)
                    matrix = self.reader.getIntMatrix(key)
                    if matrix.size > 0:  # 检查矩阵是否为空
                        self.int_matrices[key] = {
                            'shape': matrix.shape,
                            'dtype': matrix.dtype,
                            'min': np.min(matrix),
                            'max': np.max(matrix),
                            'mean': np.mean(matrix)
                        }
                        logger.info(f"整型矩阵 {key}:")
                        logger.info(f"  形状: {matrix.shape}")
                        logger.info(f"  数据类型: {matrix.dtype}")
                        logger.info(f"  数值范围: [{np.min(matrix)}, {np.max(matrix)}]")
                        logger.info(f"  平均值: {np.mean(matrix)}")
                    else:
                        logger.warning(f"整型矩阵 {key} 为空")
                except Exception as e:
                    logger.warning(f"读取整型矩阵 {i} 时出错: {str(e)}")
                    # 尝试获取更多信息
                    try:
                        logger.info(f"  矩阵形状: {matrix.shape}")
                        logger.info(f"  矩阵类型: {type(matrix)}")
                    except:
                        pass

        except Exception as e:
            logger.error(f"读取矩阵信息时出错: {str(e)}")

    def _read_image_info(self):
        """读取图像信息"""
        try:
            image_count = self.reader.getImageCount()
            logger.info(f"\n图像数量: {image_count}")

            if image_count > 0:
                # 读取第一张图像的信息
                image = self.reader.getImage(0)

                # 确保有正确的尺寸信息
                if self.spatial_size is None or self.spectral_size is None:
                    raise ValueError("缺少图像尺寸信息")

                # 重塑图像数组
                image = image.view()
                image.shape = (self.spectral_size, self.spatial_size)

                self.image_info = {
                    'count': image_count,
                    'shape': image.shape,
                    'dtype': image.dtype,
                    'offset': self.reader.getImageOffsetInBytes()
                }

                logger.info("图像信息:")
                logger.info(f"  数量: {image_count}")
                logger.info(f"  形状: {image.shape}")
                logger.info(f"  数据类型: {image.dtype}")
                logger.info(f"  文件偏移量: {self.reader.getImageOffsetInBytes()} bytes")

                # 显示第一张图像的基本统计信息
                logger.info(f"  数值范围: [{np.min(image)}, {np.max(image)}]")
                logger.info(f"  平均值: {np.mean(image)}")
                logger.info(f"  标准差: {np.std(image)}")

        except Exception as e:
            logger.error(f"读取图像信息时出错: {str(e)}")

    def _read_raw_header(self):
        """读取原始头部信息"""
        try:
            header = self.reader.getRawBinaryHeader()
            if header:
                logger.info("\n原始头部信息:")
                logger.info(f"  头部大小: {len(header)} bytes")
                # 将头部数据转换为十六进制字符串
                hex_header = ' '.join([f'{ord(b):02x}' for b in header])
                logger.info(f"  头部内容(十六进制): {hex_header}")
        except Exception as e:
            logger.error(f"读取原始头部信息时出错: {str(e)}")

    def display_image(self, index=0, display_mode='all'):
        """显示指定索引的图像
        Args:
            index: 图像索引
            display_mode: 显示模式，可选 'all', 'single', 'rgb', 'spectral'
        """
        try:
            if not self.reader or index >= self.image_info['count']:
                raise ValueError("无效的图像索引")

            # 读取并重塑图像
            image = self.reader.getImage(index).view()
            image.shape = (self.spectral_size, self.spatial_size)

            if display_mode in ['all', 'single']:
                # 1. 显示单波段图像
                plt.figure(figsize=(12, 8))
                plt.imshow(image, cmap='viridis')
                plt.colorbar(label='DN值')
                plt.title(f'HYSPEX图像 #{index + 1} (单波段显示)')
                plt.xlabel('空间位置')
                plt.ylabel('光谱波段')
                plt.tight_layout()
                plt.show()

            if display_mode in ['all', 'rgb']:
                # 2. 显示RGB合成图像
                # 选择三个波段作为RGB通道
                r_band = self.spectral_size // 3
                g_band = self.spectral_size // 2
                b_band = 2 * self.spectral_size // 3

                rgb_image = np.zeros((self.spatial_size, 3))
                rgb_image[:, 0] = image[r_band, :]  # R通道
                rgb_image[:, 1] = image[g_band, :]  # G通道
                rgb_image[:, 2] = image[b_band, :]  # B通道

                # 归一化
                rgb_image = (rgb_image - np.min(rgb_image)) / (np.max(rgb_image) - np.min(rgb_image))

                plt.figure(figsize=(12, 8))
                plt.imshow(rgb_image.reshape(1, -1, 3), aspect='auto')
                plt.title(f'HYSPEX图像 #{index + 1} (RGB合成)')
                plt.xlabel('空间位置')
                plt.ylabel('RGB合成')
                plt.tight_layout()
                plt.show()

            if display_mode in ['all', 'spectral']:
                # 3. 显示光谱曲线
                # 选择几个空间位置显示其光谱曲线
                positions = [0, self.spatial_size // 4, self.spatial_size // 2, 3 * self.spatial_size // 4,
                             self.spatial_size - 1]

                plt.figure(figsize=(12, 8))
                for pos in positions:
                    spectral_curve = image[:, pos]
                    plt.plot(spectral_curve, label=f'位置 {pos}')

                plt.title(f'HYSPEX图像 #{index + 1} (光谱曲线)')
                plt.xlabel('光谱波段')
                plt.ylabel('DN值')
                plt.legend()
                plt.grid(True)
                plt.tight_layout()
                plt.show()

        except Exception as e:
            logger.error(f"显示图像时出错: {str(e)}")

    def display_band_dimension(self):
        """将所有图像叠加，并显示波段维度的分布"""
        try:
            image_count = self.image_info['count']
            logger.info(f"\n开始处理所有图像，共 {image_count} 张")
            
            # 创建三维数组来存储所有图像
            all_images = np.zeros((image_count, self.spectral_size, self.spatial_size), dtype=np.uint16)
            
            # 读取所有图像
            for i in range(image_count):
                image = self.reader.getImage(i).view()
                image.shape = (self.spectral_size, self.spatial_size)
                all_images[i] = image
            
            # 计算波段维度的统计信息
            band_means = np.mean(all_images, axis=(0, 2))  # 对时间和空间维度取平均
            band_stds = np.std(all_images, axis=(0, 2))    # 对时间和空间维度取标准差
            
            # 创建图像显示
            plt.figure(figsize=(15, 10))
            
            # 绘制波段均值曲线
            plt.subplot(2, 1, 1)
            plt.plot(band_means, label='波段均值')
            plt.title('所有图像在不同波段的平均响应')
            plt.xlabel('波段索引')
            plt.ylabel('平均DN值')
            plt.grid(True)
            plt.legend()
            
            # 绘制波段标准差曲线
            plt.subplot(2, 1, 2)
            plt.plot(band_stds, label='波段标准差')
            plt.title('所有图像在不同波段的标准差')
            plt.xlabel('波段索引')
            plt.ylabel('标准差')
            plt.grid(True)
            plt.legend()
            
            plt.tight_layout()
            plt.show()
            
            # 打印一些统计信息
            logger.info("\n波段维度统计信息:")
            logger.info(f"  波段均值范围: [{np.min(band_means):.2f}, {np.max(band_means):.2f}]")
            logger.info(f"  波段标准差范围: [{np.min(band_stds):.2f}, {np.max(band_stds):.2f}]")
            
            # 计算相邻图像之间的相关性
            correlations = []
            for i in range(image_count-1):
                corr = np.corrcoef(all_images[i].flatten(), all_images[i+1].flatten())[0,1]
                correlations.append(corr)
            
            logger.info(f"\n相邻图像相关性统计:")
            logger.info(f"  平均相关性: {np.mean(correlations):.4f}")
            logger.info(f"  最大相关性: {np.max(correlations):.4f}")
            logger.info(f"  最小相关性: {np.min(correlations):.4f}")
            
        except Exception as e:
            logger.error(f"显示波段维度时出错: {str(e)}")

    def display_band_slice(self, band_index=None):
        """显示特定波段下所有图像的空间分布
        Args:
            band_index: 要显示的波段索引，如果为None则显示中间波段
        """
        try:
            image_count = self.image_info['count']
            if band_index is None:
                band_index = self.spectral_size // 2  # 默认显示中间波段
            
            if band_index >= self.spectral_size:
                raise ValueError(f"波段索引超出范围: {band_index} >= {self.spectral_size}")
            
            logger.info(f"\n开始处理所有图像，共 {image_count} 张，显示波段 {band_index}")
            
            # 创建二维数组来存储选定波段的所有图像
            band_slice = np.zeros((image_count, self.spatial_size), dtype=np.uint16)
            
            # 读取所有图像的选定波段
            for i in range(image_count):
                image = self.reader.getImage(i).view()
                image.shape = (self.spectral_size, self.spatial_size)
                band_slice[i] = image[band_index, :]
            
            # 显示图像
            plt.figure(figsize=(15, 8))
            # 计算合适的显示比例
            aspect_ratio = self.spatial_size / image_count
            plt.imshow(band_slice, cmap='viridis', aspect=aspect_ratio)
            plt.colorbar(label='DN值')
            plt.title(f'波段 {band_index} 的空间分布')
            plt.xlabel('空间位置')
            plt.ylabel('图像索引')
            plt.tight_layout()
            plt.show()
            
            # 打印一些统计信息
            logger.info(f"\n波段 {band_index} 的统计信息:")
            logger.info(f"  数值范围: [{np.min(band_slice)}, {np.max(band_slice)}]")
            logger.info(f"  平均值: {np.mean(band_slice):.2f}")
            logger.info(f"  标准差: {np.std(band_slice):.2f}")
            logger.info(f"  显示比例: {aspect_ratio:.2f}")
            
        except Exception as e:
            logger.error(f"显示波段切片时出错: {str(e)}")

    def plot_spectral_curve(self, spatial_position=None):
        """生成与spectral_plot.py相同格式的光谱曲线
        Args:
            spatial_position: 空间位置索引，如果为None则使用中间位置
        """
        try:
            if not self.reader:
                raise ValueError("未打开文件")

            # 如果没有指定空间位置，使用中间位置
            if spatial_position is None:
                spatial_position = self.spatial_size // 2

            # 读取第一张图像
            image = self.reader.getImage(512).view()
            image.shape = (self.spectral_size, self.spatial_size)

            # 获取指定位置的光谱数据
            spectral = image[:, spatial_position]

            # 归一化处理
            spectral = (spectral - np.min(spectral)) / (np.max(spectral) - np.min(spectral))

            # 创建描述信息
            description = f"HYSPEX光谱数据 (空间位置: {spatial_position})"

            # 绘制光谱曲线
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

            # 打印数据信息
            logger.info(f"\n光谱数据信息 (空间位置: {spatial_position}):")
            logger.info(f"  数据点数: {len(spectral)}")
            logger.info(f"  数据范围: [{np.min(spectral):.4f}, {np.max(spectral):.4f}]")
            logger.info(f"  平均值: {np.mean(spectral):.4f}")
            logger.info(f"  标准差: {np.std(spectral):.4f}")

        except Exception as e:
            logger.error(f"生成光谱曲线时出错: {str(e)}")

    def close(self):
        """关闭文件读取器"""
        try:
            if self.reader:
                del self.reader
                self.reader = None
                logger.info("\n已关闭文件读取器")
        except Exception as e:
            logger.error(f"关闭读取器时出错: {str(e)}")


def main():
    # 文件路径
    file_path = r"E:\software_devolpment\XXW-1  2  3_Baldur_S-384N_SN12013_254us_2025-03-24T024057_raw.hyspex"

    try:
        analyzer = HySpexFileAnalyzer()
        if analyzer.open_file(file_path):
            # 显示第一张图像的所有可视化方式
            analyzer.display_image(0, display_mode='all')
            # 显示波段维度的分布
            analyzer.display_band_dimension()
            # 显示特定波段的空间分布
            analyzer.display_band_slice(band_index=100)  # 显示第100个波段
            # 生成与spectral_plot相同格式的光谱曲线
            analyzer.plot_spectral_curve(spatial_position=192)  # 显示中间位置的光谱曲线
    except Exception as e:
        logger.error(f"程序执行出错: {str(e)}")
    finally:
        analyzer.close()


if __name__ == "__main__":
    main()
