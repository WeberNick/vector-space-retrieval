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
        data: data,
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

    const { data } = this.state;

    if (data) {
      var lineData = {
        labels: data[0].queries.map(query => {
          return query.name;
        }),
        datasets: [
          {
            label: 'Cluster',
            fillColor: 'rgba(231, 76, 60,0.5)',
            strokeColor: 'rgba(231, 76, 60,0.8)',
            highlightFill: 'rgba(231, 76, 60,0.75)',
            highlightStroke: 'rgba(231, 76, 60,1)',
            pointHighlightFill: 'rgba(231, 76, 60,1)',
            data: data[0].queries.map(query => {
              return query.time;
            }),
          },
          {
            label: 'Vanilla',
            fillColor: 'rgba(46, 204, 115,0.5)',
            strokeColor: 'rgba(46, 204, 115,0.8)',
            highlightFill: 'rgba(46, 204, 115,0.75)',
            highlightStroke: 'rgba(46, 204, 115,1)',
            pointHighlightFill: 'rgba(46, 204, 115,1)',
            data: data[1].queries.map(query => {
              return query.time;
            }),
          },
          {
            label: 'Tiered',
            fillColor: 'rgba(52, 153, 219,0.5)',
            strokeColor: 'rgba(52, 153, 219,0.8)',
            highlightFill: 'rgba(52, 153, 219,0.75)',
            highlightStroke: 'rgba(52, 153, 219,1)',
            pointHighlightFill: 'rgba(52, 153, 219,1)',
            data: data[2].queries.map(query => {
              return query.time;
            }),
          },
        ],
      };

      var barData = {
        labels: ['Type'],
        datasets: [
          {
            label: 'Cluster',
            fillColor: 'rgba(231, 76, 60,0.5)',
            strokeColor: 'rgba(231, 76, 60,0.8)',
            highlightFill: 'rgba(231, 76, 60,0.75)',
            highlightStroke: 'rgba(231, 76, 60,1)',
            data: [
              data[0].queries.reduce((sum, current) => {
                return sum + current.time;
              }, 0) / data[0].queries.length,
            ],
          },
          {
            label: 'Vanilla',
            fillColor: 'rgba(46, 204, 115,0.5)',
            strokeColor: 'rgba(46, 204, 115,0.8)',
            highlightFill: 'rgba(46, 204, 115,0.75)',
            highlightStroke: 'rgba(46, 204, 115,1)',
            data: [
              data[1].queries.reduce((sum, current) => {
                return sum + current.time;
              }, 0) / data[1].queries.length,
            ],
          },
          {
            label: 'Tiered',
            fillColor: 'rgba(52, 153, 219,0.5)',
            strokeColor: 'rgba(52, 153, 219,0.8)',
            highlightFill: 'rgba(52, 153, 219,0.75)',
            highlightStroke: 'rgba(52, 153, 219,1)',
            data: [
              data[2].queries.reduce((sum, current) => {
                return sum + current.time;
              }, 0) / data[2].queries.length,
            ],
          },
        ],
      };

      return (
        <div>
          <h2 className="title is-2">Perfomance per query</h2>
          <LineChart
            data={lineData}
            ref={element => (this.lineChart = element)}
            options={{
              scaleShowGridLines: true,
              datasetFill: false,
              responsive: true,
              maintainAspectRatio: false,
              scaleLabel: f => {
                return f.value + ' sec';
              },
            }}
          />
          <div dangerouslySetInnerHTML={{ __html: legendLine }} />

          <h2 className="title is-2">Perfomance per type averaged</h2>
          <BarChart
            data={barData}
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
