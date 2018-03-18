function Dist = graddist(A, B, nn, patchsize, BIsDarker)

% Input image A,B are hxwx1, floats in [0, 1] or uint8.
% Input fields are:
% ann = nnmex(A, B) which is hxwx3 or hxwx5 if rotation+scale used

Dist = zeros(size(A));
rad = floor(patchsize/2);

% fprintf("\nSize A1, A2 = %d %d \n", size(A,1), size(A,2));

maxabs = -realmax;

for y = rad+1 : size(A, 1)-rad
    for x = rad+1 : size(A, 2)-rad
        tmp = 0;
        % ind = x + y * size(A, 2)
        bx = nn(y,x,1);
        by = nn(y,x,2);
        if bx <= rad || by <= rad || bx > size(A,2)-rad || by > size(A,1)-rad
            continue
        end
        
        for dy = -rad : rad
            for dx = -rad : rad
               % fprintf("bx = %d, by = %d, x = %d, y = %d, dx = %d, dy = %d\n", ...
               %     bx, by, x, y, dx, dy); 
                %tmp = tmp + abs(B(by+dy,bx+dx)) - abs(A(y+dy,x+dx));
                if abs(B(by+dy, bx+dx)) - abs(A(y+dy, x+dx)) > tmp
                    tmp = abs(B(by+dy, bx+dx)) - abs(A(y+dy, x+dx));
                end
                % fprintf("tmp size = %d\n", size(tmp));
            end
        end
        %tmp = tmp / (patchsize^2);
        if tmp > maxabs
            maxabs = tmp;
        end

        Dist(y,x) = tmp;
    end
end

Dist = Dist / maxabs;  % normilize to the range [-1,1]