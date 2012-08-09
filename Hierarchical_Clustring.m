
%Author: Soufiane Aq%
%Hierarchical Clustring_Simple%


Data=load('PutDataFileHere');
[ROW1,COL1]=size(Data);
distance=sqrt(sum(abs(repmat(permute(Data, [1 3 2]), [1 ROW1 1]) - repmat(permute(Data, [3 1 2]), [ROW1 1 1])).^2,3));
distance=distance+diag(ones(1,ROW1).*Inf);
k=7;
[ROW,COL] = size(distance);

 
my_clsts=num2cell(1:ROW); 

 
for i = 2:(ROW-k+1)

   
   %what is the closest cluster?
   [MinRow, IndexRow] = min(distance);
   [temp, Min_spec_col] = min(MinRow);
   Min_spec_row = IndexRow(Min_spec_col);

   
    if Min_spec_row > Min_spec_col
      t=Min_spec_row;
      Min_spec_row=Min_spec_col;
      Min_spec_col=t;
   end

my_my_clsts{Min_spec_row} = [my_clsts{Min_spec_row} my_clsts{Min_spec_col}];
my_clsts(Min_spec_col) = [];
distance(:, Min_spec_row) = min(distance(:, Min_spec_row), distance(:, Min_spec_col)); 
distance(Min_spec_row, :) = min(distance(Min_spec_row, :), distance(Min_spec_col, :)); 
distance(Min_spec_col, :) = [];
distance(:, Min_spec_col) = [];
distance(Min_spec_row, Min_spec_row) = Inf;

   
end

labels = zeros(ROW1,1);
for cc = 1:length(my_clsts)
    labels(my_clsts{cc}) = cc;
end
 
maximum_clustering=k;
clusteringData=Data;
% Cluster Plotting
for c=1:maximum_clustering
    j=1; x = 0; y = 0;
    for i=1:length(labels)
        if (labels(i) == c)
            x(j) = clusteringData(i, 1);
            y(j) = clusteringData(i, 2);
            j = j+1;
        end
    end
    plot(x,y,'o');
    hold all;
end
title('Hierarchical Clustering');
hold off;









