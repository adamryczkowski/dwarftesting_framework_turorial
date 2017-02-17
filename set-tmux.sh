#!/bin/sh

byobu-tmux new-session -d -s propoze-docs -n code 'mc mkdocs/docs mkdocs' \; split-window -p 60 -h 'sh -c "cd mkdocs/docs; ls; bash"' \; select-pane -L \; split-window -v -p 30  'mkdir -p build; cd build; cmake ..; make; ./live_docs.sh'  
