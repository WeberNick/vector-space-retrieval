import * as Chartjs from 'react-chartjs';

import React, { Component } from 'react';
import { ScrollSync, ScrollSyncPane } from 'react-scroll-sync';

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
      data.map(queryType => {
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
              data: [queryType.modes[0].average_rnt],
            },
            {
              label: queryType.modes[1].name,
              fillColor: 'rgba(231, 76, 60,0.5)',
              strokeColor: 'rgba(231, 76, 60,0.8)',
              highlightFill: 'rgba(231, 76, 60,0.75)',
              highlightStroke: 'rgba(231, 76, 60,1)',
              data: [queryType.modes[1].average_rnt],
            },
            {
              label: queryType.modes[2].name,
              fillColor: 'rgba(231, 76, 60,0.5)',
              strokeColor: 'rgba(231, 76, 60,0.8)',
              highlightFill: 'rgba(231, 76, 60,0.75)',
              highlightStroke: 'rgba(231, 76, 60,1)',
              data: [queryType.modes[2].average_rnt],
            },
            {
              label: queryType.modes[3].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [queryType.modes[3].average_rnt],
            },
            {
              label: queryType.modes[4].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [queryType.modes[4].average_rnt],
            },
            {
              label: queryType.modes[5].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [queryType.modes[5].average_rnt],
            },
            {
              label: queryType.modes[6].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [queryType.modes[6].average_rnt],
            },
            {
              label: queryType.modes[7].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [queryType.modes[7].average_rnt],
            },
            {
              label: queryType.modes[8].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [queryType.modes[8].average_rnt],
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
              data: [queryType.modes[0].average_acc],
            },
            {
              label: queryType.modes[1].name,
              fillColor: 'rgba(231, 76, 60,0.5)',
              strokeColor: 'rgba(231, 76, 60,0.8)',
              highlightFill: 'rgba(231, 76, 60,0.75)',
              highlightStroke: 'rgba(231, 76, 60,1)',
              data: [queryType.modes[1].average_acc],
            },
            {
              label: queryType.modes[2].name,
              fillColor: 'rgba(231, 76, 60,0.5)',
              strokeColor: 'rgba(231, 76, 60,0.8)',
              highlightFill: 'rgba(231, 76, 60,0.75)',
              highlightStroke: 'rgba(231, 76, 60,1)',
              data: [queryType.modes[2].average_acc],
            },
            {
              label: queryType.modes[3].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [queryType.modes[3].average_acc],
            },
            {
              label: queryType.modes[4].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [queryType.modes[4].average_acc],
            },
            {
              label: queryType.modes[5].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [queryType.modes[5].average_acc],
            },
            {
              label: queryType.modes[6].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [queryType.modes[6].average_acc],
            },
            {
              label: queryType.modes[7].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [queryType.modes[7].average_acc],
            },
            {
              label: queryType.modes[8].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [queryType.modes[8].average_acc],
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
              data: [queryType.modes[0].average_avp],
            },
            {
              label: queryType.modes[1].name,
              fillColor: 'rgba(231, 76, 60,0.5)',
              strokeColor: 'rgba(231, 76, 60,0.8)',
              highlightFill: 'rgba(231, 76, 60,0.75)',
              highlightStroke: 'rgba(231, 76, 60,1)',
              data: [queryType.modes[1].average_avp],
            },
            {
              label: queryType.modes[2].name,
              fillColor: 'rgba(231, 76, 60,0.5)',
              strokeColor: 'rgba(231, 76, 60,0.8)',
              highlightFill: 'rgba(231, 76, 60,0.75)',
              highlightStroke: 'rgba(231, 76, 60,1)',
              data: [queryType.modes[2].average_avp],
            },
            {
              label: queryType.modes[3].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [queryType.modes[3].average_avp],
            },
            {
              label: queryType.modes[4].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [queryType.modes[4].average_avp],
            },
            {
              label: queryType.modes[5].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [queryType.modes[5].average_avp],
            },
            {
              label: queryType.modes[6].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [queryType.modes[6].average_avp],
            },
            {
              label: queryType.modes[7].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [queryType.modes[7].average_avp],
            },
            {
              label: queryType.modes[8].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [queryType.modes[8].average_avp],
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
              data: [queryType.modes[0].average_dcg],
            },
            {
              label: queryType.modes[1].name,
              fillColor: 'rgba(231, 76, 60,0.5)',
              strokeColor: 'rgba(231, 76, 60,0.8)',
              highlightFill: 'rgba(231, 76, 60,0.75)',
              highlightStroke: 'rgba(231, 76, 60,1)',
              data: [queryType.modes[1].average_dcg],
            },
            {
              label: queryType.modes[2].name,
              fillColor: 'rgba(231, 76, 60,0.5)',
              strokeColor: 'rgba(231, 76, 60,0.8)',
              highlightFill: 'rgba(231, 76, 60,0.75)',
              highlightStroke: 'rgba(231, 76, 60,1)',
              data: [queryType.modes[2].average_dcg],
            },
            {
              label: queryType.modes[3].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [queryType.modes[3].average_dcg],
            },
            {
              label: queryType.modes[4].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [queryType.modes[4].average_dcg],
            },
            {
              label: queryType.modes[5].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [queryType.modes[5].average_dcg],
            },
            {
              label: queryType.modes[6].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [queryType.modes[6].average_dcg],
            },
            {
              label: queryType.modes[7].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [queryType.modes[7].average_dcg],
            },
            {
              label: queryType.modes[8].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [queryType.modes[8].average_dcg],
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
              data: [queryType.modes[0].average_fms],
            },
            {
              label: queryType.modes[1].name,
              fillColor: 'rgba(231, 76, 60,0.5)',
              strokeColor: 'rgba(231, 76, 60,0.8)',
              highlightFill: 'rgba(231, 76, 60,0.75)',
              highlightStroke: 'rgba(231, 76, 60,1)',
              data: [queryType.modes[1].average_fms],
            },
            {
              label: queryType.modes[2].name,
              fillColor: 'rgba(231, 76, 60,0.5)',
              strokeColor: 'rgba(231, 76, 60,0.8)',
              highlightFill: 'rgba(231, 76, 60,0.75)',
              highlightStroke: 'rgba(231, 76, 60,1)',
              data: [queryType.modes[2].average_fms],
            },
            {
              label: queryType.modes[3].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [queryType.modes[3].average_fms],
            },
            {
              label: queryType.modes[4].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [queryType.modes[4].average_fms],
            },
            {
              label: queryType.modes[5].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [queryType.modes[5].average_fms],
            },
            {
              label: queryType.modes[6].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [queryType.modes[6].average_fms],
            },
            {
              label: queryType.modes[7].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [queryType.modes[7].average_fms],
            },
            {
              label: queryType.modes[8].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [queryType.modes[8].average_fms],
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
              data: [queryType.modes[0].average_pre],
            },
            {
              label: queryType.modes[1].name,
              fillColor: 'rgba(231, 76, 60,0.5)',
              strokeColor: 'rgba(231, 76, 60,0.8)',
              highlightFill: 'rgba(231, 76, 60,0.75)',
              highlightStroke: 'rgba(231, 76, 60,1)',
              data: [queryType.modes[1].average_pre],
            },
            {
              label: queryType.modes[2].name,
              fillColor: 'rgba(231, 76, 60,0.5)',
              strokeColor: 'rgba(231, 76, 60,0.8)',
              highlightFill: 'rgba(231, 76, 60,0.75)',
              highlightStroke: 'rgba(231, 76, 60,1)',
              data: [queryType.modes[2].average_pre],
            },
            {
              label: queryType.modes[3].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [queryType.modes[3].average_pre],
            },
            {
              label: queryType.modes[4].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [queryType.modes[4].average_pre],
            },
            {
              label: queryType.modes[5].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [queryType.modes[5].average_pre],
            },
            {
              label: queryType.modes[6].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [queryType.modes[6].average_pre],
            },
            {
              label: queryType.modes[7].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [queryType.modes[7].average_pre],
            },
            {
              label: queryType.modes[8].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [queryType.modes[8].average_pre],
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
              data: [queryType.modes[0].average_rec],
            },
            {
              label: queryType.modes[1].name,
              fillColor: 'rgba(231, 76, 60,0.5)',
              strokeColor: 'rgba(231, 76, 60,0.8)',
              highlightFill: 'rgba(231, 76, 60,0.75)',
              highlightStroke: 'rgba(231, 76, 60,1)',
              data: [queryType.modes[1].average_rec],
            },
            {
              label: queryType.modes[2].name,
              fillColor: 'rgba(231, 76, 60,0.5)',
              strokeColor: 'rgba(231, 76, 60,0.8)',
              highlightFill: 'rgba(231, 76, 60,0.75)',
              highlightStroke: 'rgba(231, 76, 60,1)',
              data: [queryType.modes[2].average_rec],
            },
            {
              label: queryType.modes[3].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [queryType.modes[3].average_rec],
            },
            {
              label: queryType.modes[4].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [queryType.modes[4].average_rec],
            },
            {
              label: queryType.modes[5].name,
              fillColor: 'rgba(46, 204, 115,0.5)',
              strokeColor: 'rgba(46, 204, 115,0.8)',
              highlightFill: 'rgba(46, 204, 115,0.75)',
              highlightStroke: 'rgba(46, 204, 115,1)',
              data: [queryType.modes[5].average_rec],
            },
            {
              label: queryType.modes[6].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [queryType.modes[6].average_rec],
            },
            {
              label: queryType.modes[7].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [queryType.modes[7].average_rec],
            },
            {
              label: queryType.modes[8].name,
              fillColor: 'rgba(52, 153, 219,0.5)',
              strokeColor: 'rgba(52, 153, 219,0.8)',
              highlightFill: 'rgba(52, 153, 219,0.75)',
              highlightStroke: 'rgba(52, 153, 219,1)',
              data: [queryType.modes[8].average_rec],
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

          <ScrollSync>
            <div style={{ position: 'relative', height: 300 }}>
              {queryTypeBars.map(queryTypeData => {
                return (
                  <ScrollSyncPane>
                    <div key={queryTypeData.name} className="query-type-data">
                      <div style={{ position: 'sticky', left: 0 }}>
                        <strong>{queryTypeData.name}</strong>
                      </div>
                      <div className="per-measure">
                        <p>Runtime Performance (Averaged)</p>
                        <BarChart
                          data={queryTypeData.average_rnt_per_mode}
                          ref={element => (this.barChart = element)}
                          height={250}
                          width={500}
                          options={{
                            scaleShowGridLines: true,
                            responsive: true,
                            maintainAspectRatio: true,
                            scaleLabel: f => {
                              return f.value + ' sec';
                            },
                          }}
                        />
                        <div dangerouslySetInnerHTML={{ __html: legendBar }} />
                      </div>
                      <div className="per-measure">
                        <p>Accuracy (Averaged)</p>
                        <BarChart
                          data={queryTypeData.average_acc_per_mode}
                          ref={element => (this.barChart = element)}
                          height={250}
                          width={500}
                          options={{
                            scaleShowGridLines: true,
                            responsive: true,
                            maintainAspectRatio: true,
                            scaleLabel: f => {
                              return f.value;
                            },
                          }}
                        />
                        <div dangerouslySetInnerHTML={{ __html: legendBar }} />
                      </div>
                      <div className="per-measure">
                        <p>Average Precision (Averaged)</p>
                        <BarChart
                          data={queryTypeData.average_avp_per_mode}
                          ref={element => (this.barChart = element)}
                          height={250}
                          width={500}
                          options={{
                            scaleShowGridLines: true,
                            responsive: true,
                            maintainAspectRatio: true,
                            scaleLabel: f => {
                              return f.value;
                            },
                          }}
                        />
                        <div dangerouslySetInnerHTML={{ __html: legendBar }} />
                      </div>
                      <div className="per-measure">
                        <p>nDCG (Averaged)</p>
                        <BarChart
                          data={queryTypeData.average_dcg_per_mode}
                          ref={element => (this.barChart = element)}
                          height={250}
                          width={500}
                          options={{
                            scaleShowGridLines: true,
                            responsive: true,
                            maintainAspectRatio: true,
                            scaleLabel: f => {
                              return f.value;
                            },
                          }}
                        />
                        <div dangerouslySetInnerHTML={{ __html: legendBar }} />
                      </div>

                      <div className="per-measure">
                        <p>F-Measure (Averaged)</p>
                        <BarChart
                          data={queryTypeData.average_fms_per_mode}
                          ref={element => (this.barChart = element)}
                          height={250}
                          width={500}
                          options={{
                            scaleShowGridLines: true,
                            responsive: true,
                            maintainAspectRatio: true,
                            scaleLabel: f => {
                              return f.value;
                            },
                          }}
                        />
                        <div dangerouslySetInnerHTML={{ __html: legendBar }} />
                      </div>
                      <div className="per-measure">
                        <p>Precision (Averaged)</p>
                        <BarChart
                          data={queryTypeData.average_pre_per_mode}
                          ref={element => (this.barChart = element)}
                          height={250}
                          width={500}
                          options={{
                            scaleShowGridLines: true,
                            responsive: true,
                            maintainAspectRatio: true,
                            scaleLabel: f => {
                              return f.value;
                            },
                          }}
                        />
                        <div dangerouslySetInnerHTML={{ __html: legendBar }} />
                      </div>

                      <div className="per-measure">
                        <p>Recall (Averaged)</p>
                        <BarChart
                          data={queryTypeData.average_rec_per_mode}
                          ref={element => (this.barChart = element)}
                          height={250}
                          width={500}
                          options={{
                            scaleShowGridLines: true,
                            responsive: true,
                            maintainAspectRatio: true,
                            scaleLabel: f => {
                              return f.value;
                            },
                          }}
                        />
                        <div dangerouslySetInnerHTML={{ __html: legendBar }} />
                      </div>
                    </div>
                  </ScrollSyncPane>
                );
              })}
            </div>
          </ScrollSync>
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
