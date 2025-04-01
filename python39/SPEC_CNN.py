import torch
import torch.nn as nn
import torch.nn.functional as F

class SPEC_CNN(nn.Module):
    def __init__(self, input_shape=(256, 1), num_classes=9):
        super(SPEC_CNN, self).__init__()
        self.wave_shape = 256
        
        # 初始化权重
        self.initializer = nn.init.normal_(torch.empty(256, 1), mean=0.0, std=0.5)
        
        # Block 1
        self.conv1_1 = nn.Conv1d(1, 32, kernel_size=3, stride=2, padding=1)
        self.conv1_2 = nn.Conv1d(32, 32, kernel_size=3, stride=1, padding=1)
        self.conv1_3 = nn.Conv1d(32, 32, kernel_size=3, stride=1, padding=1)
        self.pool1 = nn.MaxPool1d(kernel_size=2, stride=1)
        self.dropout1 = nn.Dropout(0.25)
        
        # Block 2
        self.conv2_1 = nn.Conv1d(32, 64, kernel_size=3, stride=2, padding=1)
        self.conv2_2 = nn.Conv1d(64, 64, kernel_size=3, stride=1, padding=1)
        self.conv2_3 = nn.Conv1d(64, 64, kernel_size=3, stride=1, padding=1)
        self.pool2 = nn.MaxPool1d(kernel_size=2, stride=1)
        self.dropout2 = nn.Dropout(0.25)
        
        # Block 3
        self.conv3_1 = nn.Conv1d(64, 128, kernel_size=3, stride=2, padding=1)
        self.conv3_2 = nn.Conv1d(128, 128, kernel_size=3, stride=1, padding=1)
        self.conv3_3 = nn.Conv1d(128, 128, kernel_size=3, stride=1, padding=1)
        self.pool3 = nn.MaxPool1d(kernel_size=2, stride=1)
        self.dropout3 = nn.Dropout(0.25)
        
        # Block 4
        self.conv4_1 = nn.Conv1d(128, 256, kernel_size=3, stride=2, padding=1)
        self.conv4_2 = nn.Conv1d(256, 256, kernel_size=3, stride=1, padding=1)
        self.conv4_3 = nn.Conv1d(256, 256, kernel_size=3, stride=1, padding=1)
        self.pool4 = nn.MaxPool1d(kernel_size=2, stride=1)
        self.dropout4 = nn.Dropout(0.25)
        
        # 全连接层
        self.fc1 = nn.Linear(256 * 16, 512)  # 输入维度需要根据实际特征图大小调整
        self.fc2 = nn.Linear(512, num_classes)
        
    def forward(self, x):
        # Block 1
        x = F.relu(self.conv1_1(x))
        x = F.relu(self.conv1_2(x))
        x = F.relu(self.conv1_3(x))
        skip1 = self.pool1(x)
        x = self.dropout1(skip1)
        
        # Block 2
        x = F.relu(self.conv2_1(x))
        x = F.relu(self.conv2_2(x))
        x = F.relu(self.conv2_3(x))
        skip2 = self.pool2(x)
        x = self.dropout2(skip2)
        
        # Block 3
        x = F.relu(self.conv3_1(x))
        x = F.relu(self.conv3_2(x))
        x = F.relu(self.conv3_3(x))
        skip3 = self.pool3(x)
        x = self.dropout3(skip3)
        
        # Block 4
        x = F.relu(self.conv4_1(x))
        x = F.relu(self.conv4_2(x))
        x = F.relu(self.conv4_3(x))
        x = self.pool4(x)
        x = self.dropout4(x)
        
        # 展平并连接skip connections
        x = x.view(x.size(0), -1)
        skip1 = skip1.view(skip1.size(0), -1)
        skip2 = skip2.view(skip2.size(0), -1)
        skip3 = skip3.view(skip3.size(0), -1)
        
        x = torch.cat([x, skip1, skip2, skip3], dim=1)
        
        # 全连接层
        x = self.fc1(x)
        x = F.relu(x)
        x = self.dropout4(x)
        x = self.fc2(x)
        x = F.softmax(x, dim=1)
        
        return x