import { Grid, media } from '../styles/styles';
import React, { Component } from 'react';
import {
  NotificationContainer,
  NotificationManager,
} from 'react-notifications';

import Box from './Box';
import socket from '../socket';
import styled from 'styled-components';

const docs = [
  {
    id: 1,
    similarity: 0.72856192,
    title:
      'Intake of fried meat and risk of cancer: a follow-up study in Finland',
    content:
      'It has been suggested that mutagens in fried meat may be involved in the cancer process. Therefore the relationships between intake of fried meat and subsequent risk of cancers at different sites were studied among 9,990 Finnish men and women, 15-99 years of age and initially free of cancer. The baseline study was carried out in 1966-1972, and cases of cancer were identified through data linkage with the Finnish Cancer Registry. During a 24-year follow-up, 853 cancer cases were diagnosed. The intake of fried meat was estimated from a dietary history interview covering the total diet of the participants during the previous year. There was a positive association between fried meat intake and the risk of female-hormone-related cancers, i.e., cancer of the breast, endometrium and ovary combined. The relative risk of these cancers combined between persons in the highest and lowest tertiles of daily intake of fried meat adjusted for age, personal characteristics and intake of other main food groups was 1.77 (95% confidence interval = 1.11-2.84). Pancreatic and nervous system cancers also presented non-significant suggestive associations. No associations were observed with respect to other single cancer sites studied or to all sites of cancer combined. Further epidemiological efforts are needed to ascertain the potential link between fried-food mutagens and cancer risk.',
  },
  {
    id: 2,
    similarity: 0.72856192,
    title:
      'Intake of fried meat and risk of cancer: a follow-up study in Finland',
    content:
      'It has been suggested that mutagens in fried meat may be involved in the cancer process. Therefore the relationships between intake of fried meat and subsequent risk of cancers at different sites were studied among 9,990 Finnish men and women, 15-99 years of age and initially free of cancer. The baseline study was carried out in 1966-1972, and cases of cancer were identified through data linkage with the Finnish Cancer Registry. During a 24-year follow-up, 853 cancer cases were diagnosed. The intake of fried meat was estimated from a dietary history interview covering the total diet of the participants during the previous year. There was a positive association between fried meat intake and the risk of female-hormone-related cancers, i.e., cancer of the breast, endometrium and ovary combined. The relative risk of these cancers combined between persons in the highest and lowest tertiles of daily intake of fried meat adjusted for age, personal characteristics and intake of other main food groups was 1.77 (95% confidence interval = 1.11-2.84). Pancreatic and nervous system cancers also presented non-significant suggestive associations. No associations were observed with respect to other single cancer sites studied or to all sites of cancer combined. Further epidemiological efforts are needed to ascertain the potential link between fried-food mutagens and cancer risk.',
  },
  {
    id: 3,
    similarity: 0.72856192,
    title:
      'Intake of fried meat and risk of cancer: a follow-up study in Finland',
    content:
      'It has been suggested that mutagens in fried meat may be involved in the cancer process. Therefore the relationships between intake of fried meat and subsequent risk of cancers at different sites were studied among 9,990 Finnish men and women, 15-99 years of age and initially free of cancer. The baseline study was carried out in 1966-1972, and cases of cancer were identified through data linkage with the Finnish Cancer Registry. During a 24-year follow-up, 853 cancer cases were diagnosed. The intake of fried meat was estimated from a dietary history interview covering the total diet of the participants during the previous year. There was a positive association between fried meat intake and the risk of female-hormone-related cancers, i.e., cancer of the breast, endometrium and ovary combined. The relative risk of these cancers combined between persons in the highest and lowest tertiles of daily intake of fried meat adjusted for age, personal characteristics and intake of other main food groups was 1.77 (95% confidence interval = 1.11-2.84). Pancreatic and nervous system cancers also presented non-significant suggestive associations. No associations were observed with respect to other single cancer sites studied or to all sites of cancer combined. Further epidemiological efforts are needed to ascertain the potential link between fried-food mutagens and cancer risk.',
  },
  {
    id: 4,
    similarity: 0.72856192,
    title:
      'Intake of fried meat and risk of cancer: a follow-up study in Finland',
    content:
      'It has been suggested that mutagens in fried meat may be involved in the cancer process. Therefore the relationships between intake of fried meat and subsequent risk of cancers at different sites were studied among 9,990 Finnish men and women, 15-99 years of age and initially free of cancer. The baseline study was carried out in 1966-1972, and cases of cancer were identified through data linkage with the Finnish Cancer Registry. During a 24-year follow-up, 853 cancer cases were diagnosed. The intake of fried meat was estimated from a dietary history interview covering the total diet of the participants during the previous year. There was a positive association between fried meat intake and the risk of female-hormone-related cancers, i.e., cancer of the breast, endometrium and ovary combined. The relative risk of these cancers combined between persons in the highest and lowest tertiles of daily intake of fried meat adjusted for age, personal characteristics and intake of other main food groups was 1.77 (95% confidence interval = 1.11-2.84). Pancreatic and nervous system cancers also presented non-significant suggestive associations. No associations were observed with respect to other single cancer sites studied or to all sites of cancer combined. Further epidemiological efforts are needed to ascertain the potential link between fried-food mutagens and cancer risk.',
  },
  {
    id: 5,
    similarity: 0.72856192,
    title:
      'Intake of fried meat and risk of cancer: a follow-up study in Finland',
    content:
      'It has been suggested that mutagens in fried meat may be involved in the cancer process. Therefore the relationships between intake of fried meat and subsequent risk of cancers at different sites were studied among 9,990 Finnish men and women, 15-99 years of age and initially free of cancer. The baseline study was carried out in 1966-1972, and cases of cancer were identified through data linkage with the Finnish Cancer Registry. During a 24-year follow-up, 853 cancer cases were diagnosed. The intake of fried meat was estimated from a dietary history interview covering the total diet of the participants during the previous year. There was a positive association between fried meat intake and the risk of female-hormone-related cancers, i.e., cancer of the breast, endometrium and ovary combined. The relative risk of these cancers combined between persons in the highest and lowest tertiles of daily intake of fried meat adjusted for age, personal characteristics and intake of other main food groups was 1.77 (95% confidence interval = 1.11-2.84). Pancreatic and nervous system cancers also presented non-significant suggestive associations. No associations were observed with respect to other single cancer sites studied or to all sites of cancer combined. Further epidemiological efforts are needed to ascertain the potential link between fried-food mutagens and cancer risk.',
  },
  {
    id: 6,
    similarity: 0.72856192,
    title:
      'Intake of fried meat and risk of cancer: a follow-up study in Finland',
    content:
      'It has been suggested that mutagens in fried meat may be involved in the cancer process. Therefore the relationships between intake of fried meat and subsequent risk of cancers at different sites were studied among 9,990 Finnish men and women, 15-99 years of age and initially free of cancer. The baseline study was carried out in 1966-1972, and cases of cancer were identified through data linkage with the Finnish Cancer Registry. During a 24-year follow-up, 853 cancer cases were diagnosed. The intake of fried meat was estimated from a dietary history interview covering the total diet of the participants during the previous year. There was a positive association between fried meat intake and the risk of female-hormone-related cancers, i.e., cancer of the breast, endometrium and ovary combined. The relative risk of these cancers combined between persons in the highest and lowest tertiles of daily intake of fried meat adjusted for age, personal characteristics and intake of other main food groups was 1.77 (95% confidence interval = 1.11-2.84). Pancreatic and nervous system cancers also presented non-significant suggestive associations. No associations were observed with respect to other single cancer sites studied or to all sites of cancer combined. Further epidemiological efforts are needed to ascertain the potential link between fried-food mutagens and cancer risk.',
  },
  {
    id: 7,
    similarity: 0.72856192,
    title:
      'Intake of fried meat and risk of cancer: a follow-up study in Finland',
    content:
      'It has been suggested that mutagens in fried meat may be involved in the cancer process. Therefore the relationships between intake of fried meat and subsequent risk of cancers at different sites were studied among 9,990 Finnish men and women, 15-99 years of age and initially free of cancer. The baseline study was carried out in 1966-1972, and cases of cancer were identified through data linkage with the Finnish Cancer Registry. During a 24-year follow-up, 853 cancer cases were diagnosed. The intake of fried meat was estimated from a dietary history interview covering the total diet of the participants during the previous year. There was a positive association between fried meat intake and the risk of female-hormone-related cancers, i.e., cancer of the breast, endometrium and ovary combined. The relative risk of these cancers combined between persons in the highest and lowest tertiles of daily intake of fried meat adjusted for age, personal characteristics and intake of other main food groups was 1.77 (95% confidence interval = 1.11-2.84). Pancreatic and nervous system cancers also presented non-significant suggestive associations. No associations were observed with respect to other single cancer sites studied or to all sites of cancer combined. Further epidemiological efforts are needed to ascertain the potential link between fried-food mutagens and cancer risk.',
  },
  {
    id: 8,
    similarity: 0.72856192,
    title:
      'Intake of fried meat and risk of cancer: a follow-up study in Finland',
    content:
      'It has been suggested that mutagens in fried meat may be involved in the cancer process. Therefore the relationships between intake of fried meat and subsequent risk of cancers at different sites were studied among 9,990 Finnish men and women, 15-99 years of age and initially free of cancer. The baseline study was carried out in 1966-1972, and cases of cancer were identified through data linkage with the Finnish Cancer Registry. During a 24-year follow-up, 853 cancer cases were diagnosed. The intake of fried meat was estimated from a dietary history interview covering the total diet of the participants during the previous year. There was a positive association between fried meat intake and the risk of female-hormone-related cancers, i.e., cancer of the breast, endometrium and ovary combined. The relative risk of these cancers combined between persons in the highest and lowest tertiles of daily intake of fried meat adjusted for age, personal characteristics and intake of other main food groups was 1.77 (95% confidence interval = 1.11-2.84). Pancreatic and nervous system cancers also presented non-significant suggestive associations. No associations were observed with respect to other single cancer sites studied or to all sites of cancer combined. Further epidemiological efforts are needed to ascertain the potential link between fried-food mutagens and cancer risk.',
  },
  {
    id: 9,
    similarity: 0.72856192,
    title:
      'Intake of fried meat and risk of cancer: a follow-up study in Finland',
    content:
      'It has been suggested that mutagens in fried meat may be involved in the cancer process. Therefore the relationships between intake of fried meat and subsequent risk of cancers at different sites were studied among 9,990 Finnish men and women, 15-99 years of age and initially free of cancer. The baseline study was carried out in 1966-1972, and cases of cancer were identified through data linkage with the Finnish Cancer Registry. During a 24-year follow-up, 853 cancer cases were diagnosed. The intake of fried meat was estimated from a dietary history interview covering the total diet of the participants during the previous year. There was a positive association between fried meat intake and the risk of female-hormone-related cancers, i.e., cancer of the breast, endometrium and ovary combined. The relative risk of these cancers combined between persons in the highest and lowest tertiles of daily intake of fried meat adjusted for age, personal characteristics and intake of other main food groups was 1.77 (95% confidence interval = 1.11-2.84). Pancreatic and nervous system cancers also presented non-significant suggestive associations. No associations were observed with respect to other single cancer sites studied or to all sites of cancer combined. Further epidemiological efforts are needed to ascertain the potential link between fried-food mutagens and cancer risk.',
  },
];

const SearchHeader = styled.div`
  display: grid;
  grid-template-columns: 1fr 0.1fr 1fr;
  grid-template-areas: 'left divider right';
  margin-bottom: 50px;
  ${media.desktop`
    grid-template-areas: 
    'right'
    'left';
    grid-template-columns: 1fr;
    grid-gap: 1rem;
  `};
`;

const SearchBarBox = styled.div`
  grid-area: left;
`;

const AppConfigBox = styled.div`
  grid-area: right;
  input {
    margin-right: 5px;
  }
`;

class Search extends Component {
  state = {
    searching: false,
    running: false,
    ready: false,
    result: [],
    search_value: '',
    config: {
      number_tiers: 10,
      number_clusters: 10,
      vsm_type: 'kVANILLA',
      lsh: false,
      lsh_dim: 0,
      number_results: 10,
    },
  };

  componentDidMount() {
    socket.on('server:returnStatus', data => {
      this.setState({
        running: data.running,
        ready: data.ready,
        config: {
          number_tiers: data.config.number_tiers
            ? data.config.number_tiers
            : 10,
          number_clusters: data.config.number_clusters
            ? data.config.number_clusters
            : 10,
          vsm_type: data.config.vsm_type ? data.config.vsm_type : 'kVANILLA',
          lsh: data.config.lsh ? data.config.lsh : false,
          lsh_dim: data.config.lsh_dim ? data.config.lsh_dim : 0,
          number_results: data.config.number_results
            ? data.config.number_results
            : 10,
        },
      });
    });

    socket.on('server:searchResult', data => {
      console.log('Got result data:' + data);
      this.setState({
        searching: true,
        result: data,
      });
      this.stopSearch();
    });

    socket.on('server:server:searchError', data => {
      NotificationManager.warning(data, 'Warning', 3000);
      this.stopSearch();
    });

    socket.emit('client:requestStatus', {});
  }

  startSearch = () => {
    if (!this.state.ready) {
      return;
    }
    this.setState({
      searching: true,
    });
    socket.emit('client:search', {
      query: this.state.search_value,
      topK: this.state.config.number_results,
      mode: this.state.config.vsm_type,
    });
  };

  stopSearch = () => {
    this.setState({
      searching: false,
    });
  };

  startApplication = () => {
    socket.emit('client:init', this.state.config);
  };

  stopApplication = () => {
    socket.emit('client:stop');
  };

  handleConfigChange = e => {
    var config = { ...this.state.config };

    const target = e.target;
    const value = target.type === 'checkbox' ? target.checked : target.value;
    const name = target.name;

    if (!isNaN(parseInt(value)) && isFinite(value)) {
      config[name] = parseInt(value);
    } else {
      config[name] = value;
    }

    this.setState({
      config: config,
    });
  };

  handleSearchValueChange = e => {
    this.setState({
      search_value: e.target.value,
    });
  };

  render() {
    const { searching } = this.state;

    return (
      <div>
        <SearchHeader>
          <SearchBarBox>
            <h4 className="title is-4">Search</h4>
            <div className="field has-addons">
              <p className="control has-icons-left is-expanded">
                <input
                  className="input"
                  type="email"
                  name="search_value"
                  value={this.state.search_value}
                  placeholder="Search medical docs"
                  disabled={!this.state.ready}
                  onChange={this.handleSearchValueChange}
                />
                <span className="icon is-small is-left">
                  <i className="fas fa-search" />
                </span>
              </p>
              <div className="control">
                <a
                  className={
                    searching ? 'button is-info is-loading' : 'button is-info'
                  }
                  onClick={this.startSearch}
                  disabled={!this.state.ready}
                >
                  Search
                </a>
              </div>
            </div>
          </SearchBarBox>
          <div className="is-divider-vertical" />
          <AppConfigBox>
            <h4 className="title is-4">Config</h4>
            <div className="field is-horizontal">
              <div className="field-label">
                <label className="label">Type</label>
              </div>
              <div className="field-body">
                <div className="field is-narrow">
                  <div className="control">
                    <label className="radio">
                      <input
                        type="radio"
                        name="vsm_type"
                        value="kVANILLA"
                        onChange={this.handleConfigChange}
                        checked={this.state.config.vsm_type === 'kVANILLA'}
                      />
                      Vanilla
                    </label>
                    <label className="radio">
                      <input
                        type="radio"
                        name="vsm_type"
                        value="kTIERED"
                        onChange={this.handleConfigChange}
                        checked={this.state.config.vsm_type === 'kTIERED'}
                      />
                      Tiered
                    </label>
                    <label className="radio">
                      <input
                        type="radio"
                        name="vsm_type"
                        value="kCLUSTER"
                        onChange={this.handleConfigChange}
                        checked={this.state.config.vsm_type === 'kCLUSTER'}
                      />
                      Cluster
                    </label>
                  </div>
                </div>
              </div>
            </div>
            <div className="field is-horizontal">
              <div className="field-label">
                <label className="label">Use LSH</label>
              </div>
              <div className="field-body">
                <div className="field is-narrow">
                  <div className="control">
                    <label className="checkbox">
                      <input
                        type="checkbox"
                        name="lsh"
                        defaultChecked={this.state.config.lsh}
                        onChange={this.handleConfigChange}
                      />
                    </label>
                  </div>
                </div>
              </div>
            </div>
            <div className="field is-horizontal">
              <div className="field-label">
                <label className="label">LSH Dim</label>
              </div>
              <div className="field-body">
                <div className="field is-narrow">
                  <div className="control">
                    <input
                      className="input"
                      type="number"
                      name="lsh_dim"
                      min="0"
                      value={this.state.config.lsh_dim}
                      disabled={!this.state.config.lsh}
                      onChange={this.handleConfigChange}
                      placeholder="Dimensions of LSH Vectors"
                    />
                  </div>
                </div>
              </div>
            </div>
            <div className="field is-horizontal">
              <div className="field-label">
                <label className="label">Tiers</label>
              </div>
              <div className="field-body">
                <div className="field is-narrow">
                  <div className="control">
                    <input
                      className="input"
                      type="number"
                      name="number_tiers"
                      min="0"
                      value={this.state.config.number_tiers}
                      onChange={this.handleConfigChange}
                      placeholder="Number of results to output"
                    />
                  </div>
                </div>
              </div>
            </div>
            <div className="field is-horizontal">
              <div className="field-label">
                <label className="label">Clusters</label>
              </div>
              <div className="field-body">
                <div className="field is-narrow">
                  <div className="control">
                    <input
                      className="input"
                      type="number"
                      name="number_clusters"
                      min="0"
                      value={this.state.config.number_clusters}
                      onChange={this.handleConfigChange}
                      placeholder="Number of clusters"
                    />
                  </div>
                </div>
              </div>
            </div>
            <div className="field is-horizontal">
              <div className="field-label">
                <label className="label">Results</label>
              </div>
              <div className="field-body">
                <div className="field is-narrow">
                  <div className="control">
                    <input
                      className="input"
                      type="number"
                      name="number_results"
                      min="1"
                      value={this.state.config.number_results}
                      onChange={this.handleConfigChange}
                      placeholder="Number of results to output"
                    />
                  </div>
                </div>
              </div>
            </div>
            <div className="field has-addons">
              <p className="control">
                <a
                  className="button is-success is-outlined"
                  disabled={this.state.running}
                  onClick={this.startApplication}
                >
                  <span className="icon is-small">
                    <i className="fas fa-play" />
                  </span>
                  <span>Start</span>
                </a>
              </p>
              <p className="control">
                <a
                  className="button is-danger is-outlined"
                  disabled={!this.state.running}
                  onClick={this.stopApplication}
                >
                  <span className="icon is-small">
                    <i className="fas fa-stop" />
                  </span>
                  <span>Stop</span>
                </a>
              </p>
            </div>
            <div className="field">
              <p className="control">
                <span
                  className={
                    this.state.ready
                      ? 'icon has-text-success'
                      : 'icon has-text-danger'
                  }
                >
                  <i
                    className={
                      this.state.ready
                        ? 'fas fa-check-square'
                        : 'fas fa-exclamation-triangle'
                    }
                    style={{ marginRight: '5px' }}
                  />
                </span>
                {this.state.ready ? 'Ready' : 'Not ready'}
              </p>
            </div>
          </AppConfigBox>
        </SearchHeader>
        <Grid>
          {this.state.result.map(doc => {
            return <Box key={doc.id} doc={doc} />;
          })}
        </Grid>
      </div>
    );
  }
}

export default Search;
