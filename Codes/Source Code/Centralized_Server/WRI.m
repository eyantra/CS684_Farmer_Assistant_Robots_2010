
%/********************************************************************************
% Written by:  Swanand Kulkarni, Mehadi Seid,Rajesh Kaushik
% AVR Studio Version 4.17, Build 666

% Starting Date: 20 September 2010
% End Date: 8 November 2010
%*********************************************************************************/

%/********************************************************************************

%  Copyright (c) 2010, ERTS Lab.                       -*- c -*-
%   All rights reserved.

%   Redistribution and use in source and binary forms, with or without
%   modification, are permitted provided that the following conditions are met:

   %* Redistributions of source code must retain the above copyright
    %notice, this list of conditions and the following disclaimer.

%   * Redistributions in binary form must reproduce the above copyright
%     notice, this list of conditions and the following disclaimer in
%     the documentation and/or other materials provided with the
%     distribution.

%  * Neither the name of the copyright holders nor the names of
%     contributors may be used to endorse or promote products derived
%     from this software without specific prior written permission.

%   * Source code can be used for academic purpose. 
%     For commercial use permission form the author needs to be taken.

%  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
%  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
%  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
%  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
%  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
%  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
%  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
%  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
%  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
%  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
%  POSSIBILITY OF SUCH DAMAGE. 

%  Software released under Creative Commence cc by-nc-sa licence.
%  For legal information refer to: 
%  http://creativecommons.org/licenses/by-nc-sa/3.0/legalcode

%********************************************************************************/


%Worker robot controling part of the Central Server Code

function [flag]= WRI(o_p)
%vid = videoinput('winvideo',1,'YUY2_640x480') %'RGB24_640x480'); %to use in built camera
vid = videoinput('winvideo',2); %to use camera through wireless
set(vid, 'ReturnedColorSpace', 'rgb'); 

preview(vid); %preview is taken to improve the picture quality

	s = serial('COM5'); %serial port number can change dynamically
	fopen(s);
    
	
    index = 1;
    size_op = size(o_p);
    
    fprintf(s,'l');
    %fprintf(s,'2');
    for index=1:size_op(2) 
        posit = int2str(o_p(index));
        fprintf(s,posit); %position where object needs to be picked
    %fprintf(s,'4');
    end
    
    fprintf(s,'f'); 
%%%%%   
    for index=1:size_op(2)
        sharp_sensor_value = 0;
        while sharp_sensor_value ~= 1
            idn = fscanf(s,'%c\n');
            sharp_sensor_value = uint8(idn)
        end 
        img = getsnapshot(vid);  %this function takes the snapshot
        [height width comp] = size(img);
        img_bin = zeros(height, width);

        % logic for DIP .. values do change depending on the light conditions 
        for i=1:height
            for j=1:width
            % for TT ball values are there in comment
                if( (img(i,j,1) > 120 && img(i,j,1) < 256) &&... %150,300
                    (img(i,j,2) > 20 && img(i,j,2) < 80) &&... %100,250
                    (img(i,j,3) >= 0 && img(i,j,3) < 120) )      %0,100
                    img_bin(i,j) = 255;         
                end
            end
        end

        %cntr = centroid(img_bin)

        % logic for finding centroid 
        so=size(img_bin);
        cRow=0;cCol=0;
        n=1;
        for c=1:so(1,1)       
            for d=1:so(1,2)
               if img_bin(c,d)==255     
                    n=n+1;
                    cRow=cRow+i;
                    cCol=cCol+j;
                end
            end
        end
        cRowAvg=cRow/n;
        cColAvg=cCol/n;

        % (320,240) -> 4 , 300

        fprintf(s,'h'); % sending height
        fprintf(s,'6');

        imshow(img_bin); 
    end
    
        %p=p+1;
        %if(cntr(2) > 300 && cntr(2) < 400)
        %    imshow(img_bin);
        %elseif (cntr(2) > 400)
         %   imshow(img); % move a bit behind .. 1 more else condition < 300 -> move a bit forward
        %end
        

    
    
    fclose(s);
    stop(vid);
    delete(vid);
    clear vid;
    f=1;
    [flag]=[f];
