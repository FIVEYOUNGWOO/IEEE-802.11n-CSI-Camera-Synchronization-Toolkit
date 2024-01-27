close all;
clear all;
clc;
csi_serial = zeros(5,30,3,3); 
average_csi_file = zeros(30,3,3);
j=0;
k=0;
count = 0;

% csi_folder_path = 'C:\Users\duddn\Desktop\.';

A=dir(csi_folder_path);% Save the directory files into A
valid_entries = ~ismember({A.name}, {'.', '..'});
A = A(valid_entries);

disp(csi_folder_path); 
disp(A);

for n=1:length(A)
    if A(n).isdir==0
        file_name = fullfile(csi_folder_path,A(n).name);    
        csi_trace = read_bf_file(file_name);
        for i = 1:length(csi_trace)
            csi_entry = csi_trace{i};
            if ~isempty(csi_entry)
                count = count+1;
            end
        end
        average_files_index = round(count/5);
        % average_files_index = round(length(csi_trace)/5);
        for i = 1:length(csi_trace)
                csi_entry = csi_trace{i};
                if ~isempty(csi_entry)
                    csi = get_scaled_csi(csi_entry);
                    csi = permute(csi, [3, 1, 2]);
                    average_csi_file = mean(cat(4,average_csi_file,csi),4);
                    j = j+1;
                    if j == average_files_index
                        j=0;
                        k=k+1;
                        csi_serial(k,:,:,:) = average_csi_file;
                    end
                end
        end
        save([file_name,'.mat'], 'csi_serial', '-v7.3');
        clear csi_trace;
        k=0;
        j=0;
        count = 0;
        csi_serial = zeros(5,30,3,3); 
        average_csi_file = zeros(30,3,3);
    end
end