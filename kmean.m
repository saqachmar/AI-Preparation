% Soufiane Aq%
%Kmeanss Function FUN%
function [centroid,clust,square_error] = kmean(data, initialCentroids)

features=4; % here just put the number of features that you have.

dataSize = size(data,1);
dataDimension = size(data,features); 

if (max(size(initialCentroids))==1)
    K = initialCentroids;
    initialCentroids = data(1:K,:);
    
end

if (size(initialCentroids,2) ~= dataDimension)
    initialCentroids = initialCentroids';
end

K = size(initialCentroids,1); % Number of centroids,and thus number of clusters


centroid = initialCentroids; 
cluster_change = 1;
clust = zeros(dataSize,1);
while (cluster_change > 0)
    previousCentroid = centroid;

%initially, the error is set to 0. cluster_change is set to 0 to avoid
%exiting the loop at the first iteration.
    square_error = 0;
    cluster_change = 0;
    very_large=10^100;
    for i = 1:dataSize
         % assign each point to its closest centroid ( and compute best
         % distance)
        best = 0;
        best_distance = very_large;
        for j = 1:K
            dist = sum((data(i,:) - centroid(j,:)).^2);
            if (dist < best_distance)
                best=j;
                best_distance=dist;
            end
        end
        square_error = square_error + best_distance;

        
        if (clust(i) ~= best)
            clust(i) = best;
            cluster_change = 1;
        end
    end


    % Recompute centres for each cluster
    for i = 1:K
        total = sum(clust==i);
       
        if (total > 0)
            for j = 1:dataDimension
                centroid(i,j) = sum((clust==i).*(data(:,j))) / total;
            end
        else
            centroid(i,:) = data(floor(rand*dataSize)+1,:)+(10^-10)*randn(1,dataDimension);
        end
    end

end