import { Grid, media } from '../styles/styles';
import React, { Component } from 'react';
import {
  NotificationContainer,
  NotificationManager,
} from 'react-notifications';

import Box from './Box';
import socket from '../socket';
import styled from 'styled-components';

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

    search: {
      value: '',
      lsh: false,
      vsm_type: 'kVANILLA',
      number_results: 10,
    },
    config: {
      number_tiers: 10,
      lsh_dim: 1,
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
          lsh_dim: data.config.lsh_dim ? data.config.lsh_dim : 1,
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
      query: this.state.search.value,
      topK: this.state.search.number_results,
      mode: this.state.search.vsm_type,
      lsh: this.state.search.lsh,
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

  handleSearchConfigChange = e => {
    var search = { ...this.state.search };

    const target = e.target;
    const value = target.type === 'checkbox' ? target.checked : target.value;
    const name = target.name;

    if (!isNaN(parseInt(value)) && isFinite(value)) {
      search[name] = parseInt(value);
    } else {
      search[name] = value;
    }

    this.setState({
      search: search,
    });
  };

  render() {
    const { searching } = this.state;

    return (
      <div>
        <SearchHeader>
          <SearchBarBox>
            <h4 className="title is-4">Search</h4>

            {/*type of search*/}
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
                        disabled={!this.state.ready}
                        onChange={this.handleSearchConfigChange}
                        checked={this.state.search.vsm_type === 'kVANILLA'}
                      />
                      Vanilla
                    </label>
                    <label className="radio">
                      <input
                        type="radio"
                        name="vsm_type"
                        value="kTIERED"
                        disabled={!this.state.ready}
                        onChange={this.handleSearchConfigChange}
                        checked={this.state.search.vsm_type === 'kTIERED'}
                      />
                      Tiered
                    </label>
                    <label className="radio">
                      <input
                        type="radio"
                        name="vsm_type"
                        value="kCLUSTER"
                        disabled={!this.state.ready}
                        onChange={this.handleSearchConfigChange}
                        checked={this.state.search.vsm_type === 'kCLUSTER'}
                      />
                      Cluster
                    </label>
                  </div>
                </div>
              </div>
            </div>

            {/*use random projections*/}
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
                        disabled={!this.state.ready}
                        defaultChecked={this.state.search.lsh}
                        onChange={this.handleSearchConfigChange}
                      />
                    </label>
                  </div>
                </div>
              </div>
            </div>

            {/*topK results*/}
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
                      disabled={!this.state.ready}
                      value={this.state.search.number_results}
                      onChange={this.handleSearchConfigChange}
                      placeholder="Number of results to output"
                    />
                  </div>
                </div>
              </div>
            </div>

            <div className="field has-addons">
              <p className="control has-icons-left is-expanded">
                <input
                  className="input"
                  type="email"
                  name="value"
                  value={this.state.search.value}
                  placeholder="Search medical docs"
                  disabled={!this.state.ready}
                  onChange={this.handleSearchConfigChange}
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
                <label className="label">LSH Dim</label>
              </div>
              <div className="field-body">
                <div className="field is-narrow">
                  <div className="control">
                    <input
                      className="input"
                      type="number"
                      name="lsh_dim"
                      min="1"
                      disabled={this.state.ready}
                      value={this.state.config.lsh_dim}
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
                      disabled={this.state.ready}
                      value={this.state.config.number_tiers}
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
