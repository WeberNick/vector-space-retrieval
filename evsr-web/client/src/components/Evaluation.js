import * as Chartjs from 'react-chartjs';

import React, { Component } from 'react';

import socket from '../socket';

var LineChart = Chartjs.Line;
var BarChart = Chartjs.Bar;

class Evaluation extends Component {
  state = {
    data: null,
  };

  componentDidMount() {
    socket.on('server:returnEvalData', data => {
      this.setState({
        filename: data.filename,
        data: data.data,
      });
    });
    socket.emit('client:getEvalData', {});

    var legendLine = this.lineChart
      ? this.lineChart.getChart().generateLegend()
      : null;
    var legendBar = this.barChart
      ? this.barChart.getChart().generateLegend()
      : null;

    this.setState({
      legendLine: legendLine,
      legendBar: legendBar,
    });
  }

  render() {
    var legendLine = (this.state && this.state.legendLine) || '';
    var legendBar = (this.state && this.state.legendBar) || '';

    const { data, filename } = this.state;
   
    

    if (data) {
      
      var queryTypeBars = new Array();
      data.map((queryType) => {
        var queryTypeData = {};
        queryTypeData.name = queryType.name;

        var average_rnt_per_mode = {
          labels: ['Type'],
          datasets: [
            {
              label: queryType.modes[0].name,
              fillColor: 'rgba(231, 76, 60,0.5)',
              strokeColor: 'rgba(231, 76, 60,0.8)',
              highlightFill: 'rgba(231, 76, 60,0.75)',
              highlightStroke: 'rgba(231, 76, 60,1)',
              data: [
                (queryType.modes[0].queries.reduce((sum, current) => {
                  return sum + current.perf_rnt;
                }, 0) / queryType.modes[0].queries.length) > 0 ? (queryType.modes[0].queries.reduce((sum, current) => {
                  return sum + current.perf_rnt;
                }, 0) / queryType.modes[0].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[1].name,
              fillColor: 'rgba(231, 76, 60,0.5)',
              strokeColor: 'rgba(231, 76, 60,0.8)',
              highlightFill: 'rgba(231, 76, 60,0.75)',
              highlightStroke: 'rgba(231, 76, 60,1)',
              data: [
                (queryType.modes[1].queries.reduce((sum, current) => {
                  return sum + current.perf_rnt;
                }, 0) / queryType.modes[1].queries.length) > 0 ? (queryType.modes[1].queries.reduce((sum, current) => {
                  return sum + current.perf_rnt;
                }, 0) / queryType.modes[1].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[2].name,
              fillColor: 'rgba(231, 76, 60,0.5)',
              strokeColor: 'rgba(231, 76, 60,0.8)',
              highlightFill: 'rgba(231, 76, 60,0.75)',
              highlightStroke: 'rgba(231, 76, 60,1)',
              data: [
                (queryType.modes[2].queries.reduce((sum, current) => {
                  return sum + current.perf_rnt;
                }, 0) / queryType.modes[2].queries.length) > 0 ? (queryType.modes[2].queries.reduce((sum, current) => {
                  return sum + current.perf_rnt;
                }, 0) / queryType.modes[2].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[3].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [
                (queryType.modes[3].queries.reduce((sum, current) => {
                  return sum + current.perf_rnt;
                }, 0) / queryType.modes[3].queries.length) > 0 ? (queryType.modes[3].queries.reduce((sum, current) => {
                  return sum + current.perf_rnt;
                }, 0) / queryType.modes[3].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[4].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [
                (queryType.modes[4].queries.reduce((sum, current) => {
                  return sum + current.perf_rnt;
                }, 0) / queryType.modes[4].queries.length) > 0 ? (queryType.modes[4].queries.reduce((sum, current) => {
                  return sum + current.perf_rnt;
                }, 0) / queryType.modes[4].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[5].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [
                (queryType.modes[5].queries.reduce((sum, current) => {
                  return sum + current.perf_rnt;
                }, 0) / queryType.modes[5].queries.length) > 0 ? (queryType.modes[5].queries.reduce((sum, current) => {
                  return sum + current.perf_rnt;
                }, 0) / queryType.modes[5].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[6].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [
                (queryType.modes[6].queries.reduce((sum, current) => {
                  return sum + current.perf_rnt;
                }, 0) / queryType.modes[6].queries.length) > 0 ? (queryType.modes[6].queries.reduce((sum, current) => {
                  return sum + current.perf_rnt;
                }, 0) / queryType.modes[6].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[7].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [
                (queryType.modes[7].queries.reduce((sum, current) => {
                  return sum + current.perf_rnt;
                }, 0) / queryType.modes[7].queries.length) > 0 ? (queryType.modes[7].queries.reduce((sum, current) => {
                  return sum + current.perf_rnt;
                }, 0) / queryType.modes[7].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[8].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [
                (queryType.modes[8].queries.reduce((sum, current) => {
                  return sum + current.perf_rnt;
                }, 0) / queryType.modes[8].queries.length) > 0 ? (queryType.modes[8].queries.reduce((sum, current) => {
                  return sum + current.perf_rnt;
                }, 0) / queryType.modes[8].queries.length) : 0,
              ],
            },
          ],
        };

        var average_acc_per_mode = {
          labels: ['Type'],
          datasets: [
            {
              label: queryType.modes[0].name,
              fillColor: 'rgba(231, 76, 60,0.5)',
              strokeColor: 'rgba(231, 76, 60,0.8)',
              highlightFill: 'rgba(231, 76, 60,0.75)',
              highlightStroke: 'rgba(231, 76, 60,1)',
              data: [
                (queryType.modes[0].queries.reduce((sum, current) => {
                  return sum + current.perf_acc;
                }, 0) / queryType.modes[0].queries.length) > 0 ? (queryType.modes[0].queries.reduce((sum, current) => {
                  return sum + current.perf_acc;
                }, 0) / queryType.modes[0].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[1].name,
              fillColor: 'rgba(231, 76, 60,0.5)',
              strokeColor: 'rgba(231, 76, 60,0.8)',
              highlightFill: 'rgba(231, 76, 60,0.75)',
              highlightStroke: 'rgba(231, 76, 60,1)',
              data: [
                (queryType.modes[1].queries.reduce((sum, current) => {
                  return sum + current.perf_acc;
                }, 0) / queryType.modes[1].queries.length) > 0 ? (queryType.modes[1].queries.reduce((sum, current) => {
                  return sum + current.perf_acc;
                }, 0) / queryType.modes[1].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[2].name,
              fillColor: 'rgba(231, 76, 60,0.5)',
              strokeColor: 'rgba(231, 76, 60,0.8)',
              highlightFill: 'rgba(231, 76, 60,0.75)',
              highlightStroke: 'rgba(231, 76, 60,1)',
              data: [
                (queryType.modes[2].queries.reduce((sum, current) => {
                  return sum + current.perf_acc;
                }, 0) / queryType.modes[2].queries.length) > 0 ? (queryType.modes[2].queries.reduce((sum, current) => {
                  return sum + current.perf_acc;
                }, 0) / queryType.modes[2].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[3].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [
                (queryType.modes[3].queries.reduce((sum, current) => {
                  return sum + current.perf_acc;
                }, 0) / queryType.modes[3].queries.length) > 0 ? (queryType.modes[3].queries.reduce((sum, current) => {
                  return sum + current.perf_acc;
                }, 0) / queryType.modes[3].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[4].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [
                (queryType.modes[4].queries.reduce((sum, current) => {
                  return sum + current.perf_acc;
                }, 0) / queryType.modes[4].queries.length) > 0 ? (queryType.modes[4].queries.reduce((sum, current) => {
                  return sum + current.perf_acc;
                }, 0) / queryType.modes[4].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[5].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [
                (queryType.modes[5].queries.reduce((sum, current) => {
                  return sum + current.perf_acc;
                }, 0) / queryType.modes[5].queries.length) > 0 ? (queryType.modes[5].queries.reduce((sum, current) => {
                  return sum + current.perf_acc;
                }, 0) / queryType.modes[5].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[6].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [
                (queryType.modes[6].queries.reduce((sum, current) => {
                  return sum + current.perf_acc;
                }, 0) / queryType.modes[6].queries.length) > 0 ? (queryType.modes[6].queries.reduce((sum, current) => {
                  return sum + current.perf_acc;
                }, 0) / queryType.modes[6].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[7].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [
                (queryType.modes[7].queries.reduce((sum, current) => {
                  return sum + current.perf_acc;
                }, 0) / queryType.modes[7].queries.length) > 0 ? (queryType.modes[7].queries.reduce((sum, current) => {
                  return sum + current.perf_acc;
                }, 0) / queryType.modes[7].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[8].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [
                (queryType.modes[8].queries.reduce((sum, current) => {
                  return sum + current.perf_acc;
                }, 0) / queryType.modes[8].queries.length) > 0 ? (queryType.modes[8].queries.reduce((sum, current) => {
                  return sum + current.perf_acc;
                }, 0) / queryType.modes[8].queries.length) : 0,
              ],
            },
          ],
        };

        var average_avp_per_mode = {
          labels: ['Type'],
          datasets: [
            {
              label: queryType.modes[0].name,
              fillColor: 'rgba(231, 76, 60,0.5)',
              strokeColor: 'rgba(231, 76, 60,0.8)',
              highlightFill: 'rgba(231, 76, 60,0.75)',
              highlightStroke: 'rgba(231, 76, 60,1)',
              data: [
                (queryType.modes[0].queries.reduce((sum, current) => {
                  return sum + current.perf_avp;
                }, 0) / queryType.modes[0].queries.length) > 0 ? (queryType.modes[0].queries.reduce((sum, current) => {
                  return sum + current.perf_avp;
                }, 0) / queryType.modes[0].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[1].name,
              fillColor: 'rgba(231, 76, 60,0.5)',
              strokeColor: 'rgba(231, 76, 60,0.8)',
              highlightFill: 'rgba(231, 76, 60,0.75)',
              highlightStroke: 'rgba(231, 76, 60,1)',
              data: [
                (queryType.modes[1].queries.reduce((sum, current) => {
                  return sum + current.perf_avp;
                }, 0) / queryType.modes[1].queries.length) > 0 ? (queryType.modes[1].queries.reduce((sum, current) => {
                  return sum + current.perf_avp;
                }, 0) / queryType.modes[1].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[2].name,
              fillColor: 'rgba(231, 76, 60,0.5)',
              strokeColor: 'rgba(231, 76, 60,0.8)',
              highlightFill: 'rgba(231, 76, 60,0.75)',
              highlightStroke: 'rgba(231, 76, 60,1)',
              data: [
                (queryType.modes[2].queries.reduce((sum, current) => {
                  return sum + current.perf_avp;
                }, 0) / queryType.modes[2].queries.length) > 0 ? (queryType.modes[2].queries.reduce((sum, current) => {
                  return sum + current.perf_avp;
                }, 0) / queryType.modes[2].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[3].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [
                (queryType.modes[3].queries.reduce((sum, current) => {
                  return sum + current.perf_avp;
                }, 0) / queryType.modes[3].queries.length) > 0 ? (queryType.modes[3].queries.reduce((sum, current) => {
                  return sum + current.perf_avp;
                }, 0) / queryType.modes[3].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[4].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [
                (queryType.modes[4].queries.reduce((sum, current) => {
                  return sum + current.perf_avp;
                }, 0) / queryType.modes[4].queries.length) > 0 ? (queryType.modes[4].queries.reduce((sum, current) => {
                  return sum + current.perf_avp;
                }, 0) / queryType.modes[4].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[5].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [
                (queryType.modes[5].queries.reduce((sum, current) => {
                  return sum + current.perf_avp;
                }, 0) / queryType.modes[5].queries.length) > 0 ? (queryType.modes[5].queries.reduce((sum, current) => {
                  return sum + current.perf_avp;
                }, 0) / queryType.modes[5].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[6].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [
                (queryType.modes[6].queries.reduce((sum, current) => {
                  return sum + current.perf_avp;
                }, 0) / queryType.modes[6].queries.length) > 0 ? (queryType.modes[6].queries.reduce((sum, current) => {
                  return sum + current.perf_avp;
                }, 0) / queryType.modes[6].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[7].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [
                (queryType.modes[7].queries.reduce((sum, current) => {
                  return sum + current.perf_avp;
                }, 0) / queryType.modes[7].queries.length) > 0 ? (queryType.modes[7].queries.reduce((sum, current) => {
                  return sum + current.perf_avp;
                }, 0) / queryType.modes[7].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[8].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [
                (queryType.modes[8].queries.reduce((sum, current) => {
                  return sum + current.perf_avp;
                }, 0) / queryType.modes[8].queries.length) > 0 ? (queryType.modes[8].queries.reduce((sum, current) => {
                  return sum + current.perf_avp;
                }, 0) / queryType.modes[8].queries.length) : 0,
              ],
            },
          ],
        };

        var average_dcg_per_mode = {
          labels: ['Type'],
          datasets: [
            {
              label: queryType.modes[0].name,
              fillColor: 'rgba(231, 76, 60,0.5)',
              strokeColor: 'rgba(231, 76, 60,0.8)',
              highlightFill: 'rgba(231, 76, 60,0.75)',
              highlightStroke: 'rgba(231, 76, 60,1)',
              data: [
                (queryType.modes[0].queries.reduce((sum, current) => {
                  return sum + current.perf_dcg;
                }, 0) / queryType.modes[0].queries.length) > 0 ? (queryType.modes[0].queries.reduce((sum, current) => {
                  return sum + current.perf_dcg;
                }, 0) / queryType.modes[0].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[1].name,
              fillColor: 'rgba(231, 76, 60,0.5)',
              strokeColor: 'rgba(231, 76, 60,0.8)',
              highlightFill: 'rgba(231, 76, 60,0.75)',
              highlightStroke: 'rgba(231, 76, 60,1)',
              data: [
                (queryType.modes[1].queries.reduce((sum, current) => {
                  return sum + current.perf_dcg;
                }, 0) / queryType.modes[1].queries.length) > 0 ? (queryType.modes[1].queries.reduce((sum, current) => {
                  return sum + current.perf_dcg;
                }, 0) / queryType.modes[1].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[2].name,
              fillColor: 'rgba(231, 76, 60,0.5)',
              strokeColor: 'rgba(231, 76, 60,0.8)',
              highlightFill: 'rgba(231, 76, 60,0.75)',
              highlightStroke: 'rgba(231, 76, 60,1)',
              data: [
                (queryType.modes[2].queries.reduce((sum, current) => {
                  return sum + current.perf_dcg;
                }, 0) / queryType.modes[2].queries.length) > 0 ? (queryType.modes[2].queries.reduce((sum, current) => {
                  return sum + current.perf_dcg;
                }, 0) / queryType.modes[2].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[3].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [
                (queryType.modes[3].queries.reduce((sum, current) => {
                  return sum + current.perf_dcg;
                }, 0) / queryType.modes[3].queries.length) > 0 ? (queryType.modes[3].queries.reduce((sum, current) => {
                  return sum + current.perf_dcg;
                }, 0) / queryType.modes[3].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[4].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [
                (queryType.modes[4].queries.reduce((sum, current) => {
                  return sum + current.perf_dcg;
                }, 0) / queryType.modes[4].queries.length) > 0 ? (queryType.modes[4].queries.reduce((sum, current) => {
                  return sum + current.perf_dcg;
                }, 0) / queryType.modes[4].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[5].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [
                (queryType.modes[5].queries.reduce((sum, current) => {
                  return sum + current.perf_dcg;
                }, 0) / queryType.modes[5].queries.length) > 0 ? (queryType.modes[5].queries.reduce((sum, current) => {
                  return sum + current.perf_dcg;
                }, 0) / queryType.modes[5].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[6].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [
                (queryType.modes[6].queries.reduce((sum, current) => {
                  return sum + current.perf_dcg;
                }, 0) / queryType.modes[6].queries.length) > 0 ? (queryType.modes[6].queries.reduce((sum, current) => {
                  return sum + current.perf_dcg;
                }, 0) / queryType.modes[6].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[7].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [
                (queryType.modes[7].queries.reduce((sum, current) => {
                  return sum + current.perf_dcg;
                }, 0) / queryType.modes[7].queries.length) > 0 ? (queryType.modes[7].queries.reduce((sum, current) => {
                  return sum + current.perf_dcg;
                }, 0) / queryType.modes[7].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[8].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [
                (queryType.modes[8].queries.reduce((sum, current) => {
                  return sum + current.perf_dcg;
                }, 0) / queryType.modes[8].queries.length) > 0 ? (queryType.modes[8].queries.reduce((sum, current) => {
                  return sum + current.perf_dcg;
                }, 0) / queryType.modes[8].queries.length) : 0,
              ],
            },
          ],
        };

        var average_fms_per_mode = {
          labels: ['Type'],
          datasets: [
            {
              label: queryType.modes[0].name,
              fillColor: 'rgba(231, 76, 60,0.5)',
              strokeColor: 'rgba(231, 76, 60,0.8)',
              highlightFill: 'rgba(231, 76, 60,0.75)',
              highlightStroke: 'rgba(231, 76, 60,1)',
              data: [
                (queryType.modes[0].queries.reduce((sum, current) => {
                  return sum + current.perf_fms;
                }, 0) / queryType.modes[0].queries.length) > 0 ? (queryType.modes[0].queries.reduce((sum, current) => {
                  return sum + current.perf_fms;
                }, 0) / queryType.modes[0].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[1].name,
              fillColor: 'rgba(231, 76, 60,0.5)',
              strokeColor: 'rgba(231, 76, 60,0.8)',
              highlightFill: 'rgba(231, 76, 60,0.75)',
              highlightStroke: 'rgba(231, 76, 60,1)',
              data: [
                (queryType.modes[1].queries.reduce((sum, current) => {
                  return sum + current.perf_fms;
                }, 0) / queryType.modes[1].queries.length) > 0 ? (queryType.modes[1].queries.reduce((sum, current) => {
                  return sum + current.perf_fms;
                }, 0) / queryType.modes[1].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[2].name,
              fillColor: 'rgba(231, 76, 60,0.5)',
              strokeColor: 'rgba(231, 76, 60,0.8)',
              highlightFill: 'rgba(231, 76, 60,0.75)',
              highlightStroke: 'rgba(231, 76, 60,1)',
              data: [
                (queryType.modes[2].queries.reduce((sum, current) => {
                  return sum + current.perf_fms;
                }, 0) / queryType.modes[2].queries.length) > 0 ? (queryType.modes[2].queries.reduce((sum, current) => {
                  return sum + current.perf_fms;
                }, 0) / queryType.modes[2].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[3].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [
                (queryType.modes[3].queries.reduce((sum, current) => {
                  return sum + current.perf_fms;
                }, 0) / queryType.modes[3].queries.length) > 0 ? (queryType.modes[3].queries.reduce((sum, current) => {
                  return sum + current.perf_fms;
                }, 0) / queryType.modes[3].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[4].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [
                (queryType.modes[4].queries.reduce((sum, current) => {
                  return sum + current.perf_fms;
                }, 0) / queryType.modes[4].queries.length) > 0 ? (queryType.modes[4].queries.reduce((sum, current) => {
                  return sum + current.perf_fms;
                }, 0) / queryType.modes[4].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[5].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [
                (queryType.modes[5].queries.reduce((sum, current) => {
                  return sum + current.perf_fms;
                }, 0) / queryType.modes[5].queries.length) > 0 ? (queryType.modes[5].queries.reduce((sum, current) => {
                  return sum + current.perf_fms;
                }, 0) / queryType.modes[5].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[6].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [
                (queryType.modes[6].queries.reduce((sum, current) => {
                  return sum + current.perf_fms;
                }, 0) / queryType.modes[6].queries.length) > 0 ? (queryType.modes[6].queries.reduce((sum, current) => {
                  return sum + current.perf_fms;
                }, 0) / queryType.modes[6].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[7].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [
                (queryType.modes[7].queries.reduce((sum, current) => {
                  return sum + current.perf_fms;
                }, 0) / queryType.modes[7].queries.length) > 0 ? (queryType.modes[7].queries.reduce((sum, current) => {
                  return sum + current.perf_fms;
                }, 0) / queryType.modes[7].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[8].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [
                (queryType.modes[8].queries.reduce((sum, current) => {
                  return sum + current.perf_fms;
                }, 0) / queryType.modes[8].queries.length) > 0 ? (queryType.modes[8].queries.reduce((sum, current) => {
                  return sum + current.perf_fms;
                }, 0) / queryType.modes[8].queries.length) : 0,
              ],
            },
          ],
        };

        var average_pre_per_mode = {
          labels: ['Type'],
          datasets: [
            {
              label: queryType.modes[0].name,
              fillColor: 'rgba(231, 76, 60,0.5)',
              strokeColor: 'rgba(231, 76, 60,0.8)',
              highlightFill: 'rgba(231, 76, 60,0.75)',
              highlightStroke: 'rgba(231, 76, 60,1)',
              data: [
                (queryType.modes[0].queries.reduce((sum, current) => {
                  return sum + current.perf_pre;
                }, 0) / queryType.modes[0].queries.length) > 0 ? (queryType.modes[0].queries.reduce((sum, current) => {
                  return sum + current.perf_pre;
                }, 0) / queryType.modes[0].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[1].name,
              fillColor: 'rgba(231, 76, 60,0.5)',
              strokeColor: 'rgba(231, 76, 60,0.8)',
              highlightFill: 'rgba(231, 76, 60,0.75)',
              highlightStroke: 'rgba(231, 76, 60,1)',
              data: [
                (queryType.modes[1].queries.reduce((sum, current) => {
                  return sum + current.perf_pre;
                }, 0) / queryType.modes[1].queries.length) > 0 ? (queryType.modes[1].queries.reduce((sum, current) => {
                  return sum + current.perf_pre;
                }, 0) / queryType.modes[1].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[2].name,
              fillColor: 'rgba(231, 76, 60,0.5)',
              strokeColor: 'rgba(231, 76, 60,0.8)',
              highlightFill: 'rgba(231, 76, 60,0.75)',
              highlightStroke: 'rgba(231, 76, 60,1)',
              data: [
                (queryType.modes[2].queries.reduce((sum, current) => {
                  return sum + current.perf_pre;
                }, 0) / queryType.modes[2].queries.length) > 0 ? (queryType.modes[2].queries.reduce((sum, current) => {
                  return sum + current.perf_pre;
                }, 0) / queryType.modes[2].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[3].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [
                (queryType.modes[3].queries.reduce((sum, current) => {
                  return sum + current.perf_pre;
                }, 0) / queryType.modes[3].queries.length) > 0 ? (queryType.modes[3].queries.reduce((sum, current) => {
                  return sum + current.perf_pre;
                }, 0) / queryType.modes[3].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[4].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [
                (queryType.modes[4].queries.reduce((sum, current) => {
                  return sum + current.perf_pre;
                }, 0) / queryType.modes[4].queries.length) > 0 ? (queryType.modes[4].queries.reduce((sum, current) => {
                  return sum + current.perf_pre;
                }, 0) / queryType.modes[4].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[5].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [
                (queryType.modes[5].queries.reduce((sum, current) => {
                  return sum + current.perf_pre;
                }, 0) / queryType.modes[5].queries.length) > 0 ? (queryType.modes[5].queries.reduce((sum, current) => {
                  return sum + current.perf_pre;
                }, 0) / queryType.modes[5].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[6].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [
                (queryType.modes[6].queries.reduce((sum, current) => {
                  return sum + current.perf_pre;
                }, 0) / queryType.modes[6].queries.length) > 0 ? (queryType.modes[6].queries.reduce((sum, current) => {
                  return sum + current.perf_pre;
                }, 0) / queryType.modes[6].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[7].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [
                (queryType.modes[7].queries.reduce((sum, current) => {
                  return sum + current.perf_pre;
                }, 0) / queryType.modes[7].queries.length) > 0 ? (queryType.modes[7].queries.reduce((sum, current) => {
                  return sum + current.perf_pre;
                }, 0) / queryType.modes[7].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[8].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [
                (queryType.modes[8].queries.reduce((sum, current) => {
                  return sum + current.perf_pre;
                }, 0) / queryType.modes[8].queries.length) > 0 ? (queryType.modes[8].queries.reduce((sum, current) => {
                  return sum + current.perf_pre;
                }, 0) / queryType.modes[8].queries.length) : 0,
              ],
            },
          ],
        };

        var average_rec_per_mode = {
          labels: ['Type'],
          datasets: [
            {
              label: queryType.modes[0].name,
              fillColor: 'rgba(231, 76, 60,0.5)',
              strokeColor: 'rgba(231, 76, 60,0.8)',
              highlightFill: 'rgba(231, 76, 60,0.75)',
              highlightStroke: 'rgba(231, 76, 60,1)',
              data: [
                (queryType.modes[0].queries.reduce((sum, current) => {
                  return sum + current.perf_rec;
                }, 0) / queryType.modes[0].queries.length) > 0 ? (queryType.modes[0].queries.reduce((sum, current) => {
                  return sum + current.perf_rec;
                }, 0) / queryType.modes[0].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[1].name,
              fillColor: 'rgba(231, 76, 60,0.5)',
              strokeColor: 'rgba(231, 76, 60,0.8)',
              highlightFill: 'rgba(231, 76, 60,0.75)',
              highlightStroke: 'rgba(231, 76, 60,1)',
              data: [
                (queryType.modes[1].queries.reduce((sum, current) => {
                  return sum + current.perf_rec;
                }, 0) / queryType.modes[1].queries.length) > 0 ? (queryType.modes[1].queries.reduce((sum, current) => {
                  return sum + current.perf_rec;
                }, 0) / queryType.modes[1].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[2].name,
              fillColor: 'rgba(231, 76, 60,0.5)',
              strokeColor: 'rgba(231, 76, 60,0.8)',
              highlightFill: 'rgba(231, 76, 60,0.75)',
              highlightStroke: 'rgba(231, 76, 60,1)',
              data: [
                (queryType.modes[2].queries.reduce((sum, current) => {
                  return sum + current.perf_rec;
                }, 0) / queryType.modes[2].queries.length) > 0 ? (queryType.modes[2].queries.reduce((sum, current) => {
                  return sum + current.perf_rec;
                }, 0) / queryType.modes[2].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[3].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [
                (queryType.modes[3].queries.reduce((sum, current) => {
                  return sum + current.perf_rec;
                }, 0) / queryType.modes[3].queries.length) > 0 ? (queryType.modes[3].queries.reduce((sum, current) => {
                  return sum + current.perf_rec;
                }, 0) / queryType.modes[3].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[4].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [
                (queryType.modes[4].queries.reduce((sum, current) => {
                  return sum + current.perf_rec;
                }, 0) / queryType.modes[4].queries.length) > 0 ? (queryType.modes[4].queries.reduce((sum, current) => {
                  return sum + current.perf_rec;
                }, 0) / queryType.modes[4].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[5].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [
                (queryType.modes[5].queries.reduce((sum, current) => {
                  return sum + current.perf_rec;
                }, 0) / queryType.modes[5].queries.length) > 0 ? (queryType.modes[5].queries.reduce((sum, current) => {
                  return sum + current.perf_rec;
                }, 0) / queryType.modes[5].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[6].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [
                (queryType.modes[6].queries.reduce((sum, current) => {
                  return sum + current.perf_rec;
                }, 0) / queryType.modes[6].queries.length) > 0 ? (queryType.modes[6].queries.reduce((sum, current) => {
                  return sum + current.perf_rec;
                }, 0) / queryType.modes[6].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[7].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [
                (queryType.modes[7].queries.reduce((sum, current) => {
                  return sum + current.perf_rec;
                }, 0) / queryType.modes[7].queries.length) > 0 ? (queryType.modes[7].queries.reduce((sum, current) => {
                  return sum + current.perf_rec;
                }, 0) / queryType.modes[7].queries.length) : 0,
              ],
            },
            {
              label: queryType.modes[8].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [
                (queryType.modes[8].queries.reduce((sum, current) => {
                  return sum + current.perf_rec;
                }, 0) / queryType.modes[8].queries.length) > 0 ? (queryType.modes[8].queries.reduce((sum, current) => {
                  return sum + current.perf_rec;
                }, 0) / queryType.modes[8].queries.length) : 0,
              ],
            },
          ],
        };

        queryTypeData.average_rnt_per_mode = average_rnt_per_mode;
        queryTypeData.average_acc_per_mode = average_acc_per_mode;
        queryTypeData.average_avp_per_mode = average_avp_per_mode;
        queryTypeData.average_dcg_per_mode = average_dcg_per_mode;
        queryTypeData.average_fms_per_mode = average_fms_per_mode;
        queryTypeData.average_pre_per_mode = average_pre_per_mode;
        queryTypeData.average_rec_per_mode = average_rec_per_mode;
        queryTypeBars.push(queryTypeData);



      });

        console.log(queryTypeBars);




        


      return (
        <div>
          <h2 className="title is-1">{filename}</h2>
          {queryTypeBars.map((queryTypeData)=>{
            return (
              <div key={queryTypeData.name}>
                <h2 className="title is-2">{queryTypeData.name}</h2>

               {/* <h3 className="title is-3">Runtime Performance (Averaged)</h3>
                <BarChart
                data={queryTypeData.average_rnt_per_mode}
                ref={element => (this.barChart = element)}
                options={{
                  scaleShowGridLines: true,
                  responsive: true,
                  maintainAspectRatio: false,
                  scaleLabel: f => {
                    return f.value + ' sec';
                  },
                }}
                />
              <div dangerouslySetInnerHTML={{ __html: legendBar }} />


            
                <h3 className="title is-3">Accuracy (Averaged)</h3>
                <BarChart
                data={queryTypeData.average_acc_per_mode}
                ref={element => (this.barChart = element)}
                options={{
                  scaleShowGridLines: true,
                  responsive: true,
                  maintainAspectRatio: false,
                  scaleLabel: f => {
                    return f.value;
                  },
                }}
                />
                <div dangerouslySetInnerHTML={{ __html: legendBar }} />
                
                <h3 className="title is-3">Average Precision (Averaged)</h3>
                <BarChart
                data={queryTypeData.average_avp_per_mode}
                ref={element => (this.barChart = element)}
                options={{
                  scaleShowGridLines: true,
                  responsive: true,
                  maintainAspectRatio: false,
                  scaleLabel: f => {
                    return f.value;
                  },
                }}
                />
              <div dangerouslySetInnerHTML={{ __html: legendBar }} />*/}

               <h3 className="title is-3">nDCG (Averaged)</h3>
                <BarChart
                data={queryTypeData.average_dcg_per_mode}
                ref={element => (this.barChart = element)}
                options={{
                  scaleShowGridLines: true,
                  responsive: true,
                  maintainAspectRatio: false,
                  scaleLabel: f => {
                    return f.value;
                  },
                }}
                />
                <div dangerouslySetInnerHTML={{ __html: legendBar }} />

                {/*<h3 className="title is-3">F-Measure (Averaged)</h3>
                <BarChart
                data={queryTypeData.average_fms_per_mode}
                ref={element => (this.barChart = element)}
                options={{
                  scaleShowGridLines: true,
                  responsive: true,
                  maintainAspectRatio: false,
                  scaleLabel: f => {
                    return f.value;
                  },
                }}
                />
                <div dangerouslySetInnerHTML={{ __html: legendBar }} />

                <h3 className="title is-3">Precision (Averaged)</h3>
                <BarChart
                data={queryTypeData.average_pre_per_mode}
                ref={element => (this.barChart = element)}
                options={{
                  scaleShowGridLines: true,
                  responsive: true,
                  maintainAspectRatio: false,
                  scaleLabel: f => {
                    return f.value;
                  },
                }}
                />
                <div dangerouslySetInnerHTML={{ __html: legendBar }} />

                <h3 className="title is-3">Recall (Averaged)</h3>
                <BarChart
                data={queryTypeData.average_rec_per_mode}
                ref={element => (this.barChart = element)}
                options={{
                  scaleShowGridLines: true,
                  responsive: true,
                  maintainAspectRatio: false,
                  scaleLabel: f => {
                    return f.value;
                  },
                }}
                />
                <div dangerouslySetInnerHTML={{ __html: legendBar }} />
                */}



              </div>
              
            )



          })}



        </div>
      );
    } else {
      return (
        <div
          className="pageloader"
          style={{ top: '100vh', height: '100%', backgroundColor: '#54a0ff' }}
        >
          <span className="title">Loading Evaluation</span>
        </div>
      );
    }
  }
}

export default Evaluation;
