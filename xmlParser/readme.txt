Run the buildme.sh to build it
./buildme.sh

To run the tool:
./mpd_parser [-r/-n/-p] <mpdfile>

Where:
 -r : Use this if you are only interested in reading and printing each node/element
 -n : Use this if you want to normalize the mpd file
 -p : Use this if you want to parse each and every node/element pertaining to MPD (DASH)

./mpd_parser  <mpdfile>

Example: Run the tool to parse the mpd (run the tool on espn.mpd and capture all the parsed output into espn.debug file):
./mpd_parser -p espn.mpd &> espn.debug

These are the sample mpds:
* espn.mpd: Downloaded from: http://csm-e-dfwprd.tls1.yospace.com/csm/extlive/aegdfwprd01,/Content/DASH.abre/Live/channel(WESPNHD-1964.dfw.720.mobile)/manifest.mpd?yo.up=http://dfwlive-sponsored.akamaized.net/Content/DASH.abre/Live/channel(WESPNHD-1964.dfw.720.mobile)/
* mod_espn.mpd: modified version of espn.mpd (all nodes are moved to individual line)
* 1.mpd: Downloaded from: http://csm-e-dfwprd.tls1.yospace.com/csm/extlive/aegdfwprd01,/Content/DASH.abre/Live/channel(WESPNHD-1964.dfw.720.mobile)/manifest.mpd?yo.up=http://dfwlive-sponsored.akamaized.net/Content/DASH.abre/Live/channel(WESPNHD-1964.dfw.720.mobile)/ and each line is moved to individual line
* 2.mpd: copied from 1.mpd, but each node has a TAB based on its depth

When you run the tool with espn.mpd and mod_espn.mpd, the output should be the same (excluding "text" nodes)
It should be the same for 1.mpd and 2.mpd as well


After generating the debug output, you can open the file with vi/vim and run the command below to delete all the lines with "text"
:g/name: text /d

After doing that, 1.debug and 2.debug should exactly be same
